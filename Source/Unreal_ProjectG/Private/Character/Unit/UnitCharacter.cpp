// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Unit/UnitCharacter.h"
#include "Character/Unit/SubSystem/UnitSubsystem.h"
#include "Character/Unit/SubSystem/UnitSpawnSubsystem.h"
#include "Components/CapsuleComponent.h"
#include "Engine/AssetManager.h"
#include "DataAssets/StartUp/DataAsset_UnitStartupData.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "AIController.h"
#include "DataAssets/Unit/UnitData.h"
#include "AbilitySystem/PGCharacterAttributeSet.h"
#include "DataAssets/Unit/BranchDataAsset.h"
#include "AbilitySystem/PGAbilitySystemComponent.h"

AUnitCharacter::AUnitCharacter()
{
    // AI 컨트롤러 빙의 시점 설정
    PrimaryActorTick.bCanEverTick = false;

    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

    UCharacterMovementComponent* MovementComponent = GetCharacterMovement();
    if (MovementComponent)
    {
        MovementComponent->bUseRVOAvoidance = false;
    }

    if (UCapsuleComponent* Capsule = GetCapsuleComponent())
    {
        Capsule->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.5f));
    }

    if (USkeletalMeshComponent* CharacterMesh = GetMesh())
    {
        CharacterMesh->SetRelativeScale3D(FVector(2.5f, 2.5f, 2.5f));

        CharacterMesh->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
        CharacterMesh->SetRelativeLocation(FVector(0.0f, 0.0f, -90.0f)); 
    }

    WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));

    if (WeaponMesh)
    {
        WeaponMesh->SetupAttachment(GetMesh(), TEXT("Weapon_R"));
        WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        WeaponMesh->SetCollisionProfileName(TEXT("NoCollision"));
    }
}

void AUnitCharacter::BeginPlay()
{
    Super::BeginPlay();

    AIController = Cast<AAIController>(GetController());

    UE_LOG(LogTemp, Log, TEXT("BeginPlay"));


}

void AUnitCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);
}

void AUnitCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AUnitCharacter::PossessedBy(AController* NewController)
{
    Super::PossessedBy(NewController);

    InitUnitStartUpData();
}

void AUnitCharacter::PostInitializeComponents()
{
    Super::PostInitializeComponents();

    if (!PGAbilitySystemComponent)
    {
        PGAbilitySystemComponent = FindComponentByClass<UPGAbilitySystemComponent>();
    }

    if (!CharacterAttributeSet && PGAbilitySystemComponent)
    {

        CharacterAttributeSet = const_cast<UPGCharacterAttributeSet*>(PGAbilitySystemComponent->GetSet<UPGCharacterAttributeSet>());
    }
}

void AUnitCharacter::InitUnitStartUpData()
{
    if(CharacterStartupData.IsNull())
    {
        return;
    }
    // 비동기로 스타트업 데이터 로드 및 적용
    UAssetManager::GetStreamableManager().RequestAsyncLoad(
        CharacterStartupData.ToSoftObjectPath(),
        FStreamableDelegate::CreateLambda(
            [this]()
            {
                if (IsHidden() || !IsValid(this))
                {
                    return;
                }

                if (UDataAsset_StartupDataBase* LoadedData = CharacterStartupData.Get())
                {
                    if (PGAbilitySystemComponent)
                    {
                        LoadedData->GiveToAbilitySystemComponent(PGAbilitySystemComponent);
                    }
                    UDataAsset_UnitStartupData* StartUpData = Cast<UDataAsset_UnitStartupData>(LoadedData);

                    if (StartUpData->BranchData)
                    {
                        DetectRangeKey = StartUpData->BranchData->DetectRange;

                        AttackRangeKey = StartUpData->BranchData->AttackRange;

                        SubBTAssetKey = StartUpData->BranchData->SubBTAsset;

                        AttackMarginKey = AttackRangeKey * 0.7f;

                        if (StartUpData->BranchData->BranchTag.IsValid())
                        {
                            BranchTag = StartUpData->BranchData->BranchTag;
                        }
                    }
                    UE_LOG(LogTemp, Log, TEXT("InitUnitStartUpData"));
                    if (CharacterAttributeSet)
                    {
                        UE_LOG(LogTemp, Log, TEXT("HP : %f"), CharacterAttributeSet->GetHealth());
                    }
                    else
                    {
                        UE_LOG(LogTemp, Error, TEXT("[%s] AttributeSet이 없습니다! 블루프린트를 확인하세요."), *GetName());
                    }

                    if (StartUpData->TeamTag.IsValid())
                    {
                        TeamTag = StartUpData->TeamTag;
                    }

                    if (StartUpData->DeadMontage)
                    {
                        UnitDeadMontage = StartUpData->DeadMontage;
                    }

                    //유닛 서브시스템을 이용한 태그별 팀 설정
                    if (UUnitSubsystem* Subsystem = GetWorld()->GetSubsystem<UUnitSubsystem>())
                    {
                        Subsystem->RegisterUnit(this, TeamTag);
                    }

                    //데이터 삽입이 끝나면 델리게이트를 브로드캐스트해서 블랙보드가 값을 받기 시작함
                    if (OnUnitStartUpDataLoadedDelegate.IsBound())
                    {
                        OnUnitStartUpDataLoadedDelegate.Broadcast();
                    }
                }
            }
        )
    );
}


void AUnitCharacter::SetAttackTarget(AActor* InTargetActor)
{
    //적 베이스로 돌격하기 위한 함수, 지금은 유닛 블루프린트의 beginplay에서만 호출하는데 + 여기서만 적 베이스를 정할 수 있는데 나중ㅇ 바꿀듯????
    TargetActor = InTargetActor;
    if (!AIController)
    {
        AIController = Cast<AAIController>(GetController());
    }

    if (AIController)
    {
        UBlackboardComponent* BBComp = AIController->GetBlackboardComponent();
        if (BBComp)
        {
            BBComp->SetValueAsObject(TEXT("AttackTargetBase"), InTargetActor);
        }
    }
}

//void AUnitCharacter::Attack()
//{
//    UE_LOG(LogTemp, Warning, TEXT("Attack"));
//
//    //attack에서는 몽타주만 재생함, 노티파이랑 GAS를 이용해서 UGEExecCalc_DefaultDamageTaken에서 데미지 처리
//    if (UnitAttackMontage)
//    {
//        PlayAnimMontage(UnitAttackMontage);
//        UE_LOG(LogTemp, Warning, TEXT("PlayMontage"));
//
//    }
//}

void AUnitCharacter::OnDie()
{
    if (UUnitSpawnSubsystem* SpawnSubsystem = GetWorld()->GetSubsystem<UUnitSpawnSubsystem>())
    {
        UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
        if (AnimInstance && UnitDeadMontage)
        {
            float Duration = AnimInstance->Montage_Play(UnitDeadMontage)-0.2f;

            FTimerHandle TimerHandle;
            GetWorldTimerManager().SetTimer(TimerHandle, [SpawnSubsystem, this]()
                {
                    SpawnSubsystem->OnUnitDied(this);
                }, Duration, false);
        }
        else
        {
            SpawnSubsystem->OnUnitDied(this);
        }
    }
    else
    {
        Destroy();
    }
}

//오브젝트 풀링을 위한 함수들, 아직 미구현

void AUnitCharacter::ActivateUnit()
{
    SetActorHiddenInGame(false); // 보이게 하기
    SetActorEnableCollision(true); // 충돌 켜기
    SetActorTickEnabled(true); // 로직 다시 돌리기

    if (Controller == nullptr && AIControllerClass)
    {
        SpawnDefaultController();
    }
    else
    {
        InitUnitStartUpData();
    }
    GetCharacterMovement()->SetMovementMode(MOVE_Walking);

    if (TargetActor)
    {
        SetAttackTarget(TargetActor);
    }
}

void AUnitCharacter::DeactivateUnit()
{

    OnUnitStartUpDataLoadedDelegate.Clear();

    if (AController* OldController = GetController())
    {
        OldController->StopMovement();
        OldController->UnPossess(); 
        OldController->Destroy();   
    }

    if (UUnitSubsystem* Subsystem = GetWorld()->GetSubsystem<UUnitSubsystem>())
    {
        //유닛 서브시스템에서 정한 팀을 해제함 
        Subsystem->UnregisterUnit(this, TeamTag);
    }

    // 2. 물리/이동 초기화
    GetCharacterMovement()->StopMovementImmediately();
    GetCharacterMovement()->SetMovementMode(MOVE_None);

    // 3. 시각적 숨김
    SetActorEnableCollision(false);
    SetActorHiddenInGame(true);
    SetActorTickEnabled(false);

}


