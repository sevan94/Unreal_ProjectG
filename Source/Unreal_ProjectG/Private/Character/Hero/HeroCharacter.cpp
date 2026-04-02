// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Hero/HeroCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/Equipment/EquipmentsStorageComponent.h"
#include "Components/Combat/HeroCombatComponent.h"
#include "UI/Battle/ControlPanelWidget.h"
#include "DataAssets/StartUp/DataAsset_HeroStartupData.h"
#include "AbilitySystem/PGCharacterAttributeSet.h"
#include "AbilitySystem/PGAbilitySystemComponent.h"
#include "PGGameplayTags.h"
#include "EnhancedInputComponent.h"
#include "Mode/PGBaseGameMode.h"
#include "Character/Unit/SubSystem/UnitSubsystem.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Components/SphereComponent.h"
#include "GameFramework/PlayerStart.h"
#include "EngineUtils.h"

UE_DEFINE_GAMEPLAY_TAG(TAG_Unit_Side_Ally, "Unit_Side_Ally");
UE_DEFINE_GAMEPLAY_TAG(TAG_Unit_Side_Foe, "Unit_Side_Foe");
UE_DEFINE_GAMEPLAY_TAG(TAG_Player, "Player");

// Sets default values
AHeroCharacter::AHeroCharacter()
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false;
    MovementComponent = GetCharacterMovement();

    //화면이 회전하지 않도록 고정
    MovementComponent->bUseControllerDesiredRotation = true;
    MovementComponent->bOrientRotationToMovement = false;
    bUseControllerRotationYaw = true;

    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    SpringArm->SetupAttachment(RootComponent);

    CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
    CameraComponent->SetupAttachment(SpringArm);

    WeaponStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponStaticMesh"));
    WeaponStaticMesh->SetupAttachment(GetMesh());
    WeaponStaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    
    EquipmentsStorageComponent = CreateDefaultSubobject<UEquipmentsStorageComponent>(TEXT("EquipmentsStorageComponent"));
    HeroCombatComponent = CreateDefaultSubobject<UHeroCombatComponent>(TEXT("HeroCombatComponent"));
    
    //ResourceManager = CreateDefaultSubobject<UHeroResourceComponent>(TEXT("ResourceManager"));
    AggroCollision = CreateDefaultSubobject<USphereComponent>(TEXT("AggroCollision"));
    AggroCollision->SetupAttachment(RootComponent);
    AggroCollision->SetSphereRadius(500.f);
    AggroCollision->SetGenerateOverlapEvents(true);
}

float AHeroCharacter::GetBasicAttackSpeed_Implementation() const
{
    if (CharacterAttributeSet)
    {
        return CharacterAttributeSet->GetAttackSpeed();
    }
    return 1.f; // 기본 공격 속도 반환, 어트리뷰트셋이 없는 경우 기본값으로 1.0f 반환
}

float AHeroCharacter::GetBasicAttackRange_Implementation() const
{
    if (CharacterAttributeSet)
    {
        return CharacterAttributeSet->GetAttackRange();
    }
    return 0.0f; // 기본 공격 범위 반환, 어트리뷰트셋이 없는 경우 기본값으로 0.0f 반환
}

void AHeroCharacter::AutoMode()
{
    if (PotentialTargets.IsEmpty())
    {
        UE_LOG(LogTemp, Log, TEXT("AutoBattle Active"));
        //HeroCombatComponent->StopCombat();
        AddMovementInput(FVector::ForwardVector);
    }
    else
    {
        //HeroCombatComponent->StartCombat();
    }
}

bool AHeroCharacter::TryExecuteBasicAttack_Implementation()
{
    if (PGAbilitySystemComponent)
    {
        return PGAbilitySystemComponent->TryActivateAbilityByTag(PGGameplayTags::Input_BasicAttack);
    }
    return false;
}

bool AHeroCharacter::TryExecuteActiveSkill_Implementation()
{
    if (PGAbilitySystemComponent)
    {
        return PGAbilitySystemComponent->TryActivateAbilityByTag(PGGameplayTags::Input_ActiveSkill);
    }
    return false;
}

void AHeroCharacter::ChangeCombatMode(EHeroCombatMode NewMode)
{
    if (!HeroCombatComponent) return;

    HeroCombatComponent->SetCombatMode(NewMode);
}

void AHeroCharacter::SpawnHero()
{
    USkeletalMeshComponent* MeshComp = GetMesh();
    MeshComp->bPauseAnims = false;
    MeshComp->SetSimulatePhysics(false);
    MeshComp->SetCollisionProfileName(TEXT("CharacterMesh"));

    UE_LOG(LogTemp, Log, TEXT("Respawn Position : %f %f %f"), RespawnPosition.X, RespawnPosition.Y, RespawnPosition.Z);
    MeshComp->AttachToComponent(GetCapsuleComponent(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
    //MeshComp->SetWorldLocation(RespawnPosition);
    MeshComp->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

    SetActorLocation(RespawnPosition);

    GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    MovementComponent->SetComponentTickEnabled(true);
    MovementComponent->SetMovementMode(EMovementMode::MOVE_Walking);
    MovementComponent->Activate();
}

void AHeroCharacter::MakeHeroDead()
{
    MovementComponent->StopMovementImmediately();
    MovementComponent->DisableMovement();
    MovementComponent->SetComponentTickEnabled(false);

    GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    USkeletalMeshComponent* MeshComp = GetMesh();
    MeshComp->SetCollisionProfileName(TEXT("Ragdoll"));
    MeshComp->bPauseAnims = true;
    MeshComp->SetSimulatePhysics(true);

    //MeshComp->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);

    if (OnPlayerDied.IsBound())
    {
        OnPlayerDied.Broadcast();
    }
}

void AHeroCharacter::InitializeHero()
{
    if (PGAbilitySystemComponent)
    {
        PGAbilitySystemComponent->TryActivateAbilityByTag(PGGameplayTags::Player_Ability_Initialize);
    }

    //if (PGAbilitySystemComponent && GA_Initialize)
    //{
    //    PGAbilitySystemComponent->TryActivateAbilityByClass(GA_Initialize);
    //}
}

bool AHeroCharacter::ConsumeCost(float InCost)
{
    if (CharacterAttributeSet)
    {
        float CurrentCost = CharacterAttributeSet->GetCost();
        CharacterAttributeSet->SetCost(CurrentCost - InCost);
        if (UWorld* World = GetWorld())
        {
            // 서버 측에서 실행되는 GameMode를 가져옴
            APGBaseGameMode* GM = Cast<APGBaseGameMode>(World->GetAuthGameMode());
            if (GM)
            {
                // 소모한 코스트만큼 누적 (정수형일 경우 형변환)
                GM->SpentCost += FMath::RoundToInt(InCost);
            }
        }
        return true;
    }

    return false;
}

void AHeroCharacter::BroadCastAttributeSet()
{
    if (CharacterAttributeSet)
    {
        PGAbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
            CharacterAttributeSet->GetHealthAttribute()).AddUObject(this, &AHeroCharacter::CurrentHealthChange);
        PGAbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
            CharacterAttributeSet->GetMaxHealthAttribute()).AddUObject(this, &AHeroCharacter::MaxHealthChange);

        PGAbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
            CharacterAttributeSet->GetCostAttribute()).AddUObject(this, &AHeroCharacter::CurrentCostChange);
        PGAbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
            CharacterAttributeSet->GetMaxCostAttribute()).AddUObject(this, &AHeroCharacter::MaxCostChange);
    }
}

void AHeroCharacter::OnDie()
{
    PGAbilitySystemComponent->TryActivateAbilityByTag(PGGameplayTags::Player_Ability_Die);

    //// 주석
    //if (PGAbilitySystemComponent && GA_Die)
    //{
    //    //PGAbilitySystemComponent->TryActivateAbilityByClass(GA_Die);
    //}
    //else
    //    UE_LOG(LogTemp, Warning, TEXT("AbilitySystem Unavailable"));
}

// Called when the game starts or when spawned
void AHeroCharacter::BeginPlay()
{
    Super::BeginPlay();

    BroadCastAttributeSet();

    if (!CharacterStartupData.IsNull())
    {
        if (UDataAsset_StartupDataBase* LoadData = CharacterStartupData.LoadSynchronous())
        {
            LoadData->GiveToAbilitySystemComponent(PGAbilitySystemComponent);
        }
    }

    if (EquipmentsStorageComponent && Weapon && Armor && Accessory)
    {
        EquipmentsStorageComponent->Startup(Weapon, Armor, Accessory);
    }

    PlayerTag = TAG_Player;

    if (AggroCollision)
    {
        AggroCollision->OnComponentBeginOverlap.AddDynamic(this, &AHeroCharacter::OnOverlapBegin);
        AggroCollision->OnComponentEndOverlap.AddDynamic(this, &AHeroCharacter::OnOverlapEnd);
        UE_LOG(LogTemp, Log, TEXT("Overlap bind"));
    }

    if (UUnitSubsystem* Subsystem = GetWorld()->GetSubsystem<UUnitSubsystem>())
    {
        Subsystem->RegisterUnit(this, PGGameplayTags::Unit_Side_Ally);
    }

    //ABP 가져오기
    //AnimInstance = GetMesh()->GetAnimInstance();

    if (UWorld* world = GetWorld())
    {
        for (TActorIterator<APlayerStart> PlayerStart(world); PlayerStart; ++PlayerStart)
        {
            APlayerStart* startPoint = *PlayerStart;
            if (startPoint)
            {
                RespawnPosition = startPoint->GetActorLocation();
                UE_LOG(LogTemp, Log, TEXT("Hero : PlayerStart Found"));
            }
        }
    }
}

#pragma region Input
// Called to bind functionality to input
void AHeroCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    //UEnhancedInputComponent* enhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
    //if (enhancedInputComponent)
    //{
    //    enhancedInputComponent->BindAction(IA_Move, ETriggerEvent::Triggered, this, &AHeroCharacter::OnMovementInput);
    //    enhancedInputComponent->BindAction(IA_Attack, ETriggerEvent::Triggered, this, &AHeroCharacter::OnAttackInput);
    //}
}

//void AHeroCharacter::OnAttackInput()
//{
//    if (PGAbilitySystemComponent)
//    {
//        //PGAbilitySystemComponent->TryActivateAbilityByClass(GA_Attack);
//        PGAbilitySystemComponent->TryActivateAbilityByTag(PGGameplayTags::Player_Ability_BasicAttack);
//    }
//}
#pragma endregion

#pragma region AttributeChangeBroadCast
void AHeroCharacter::CurrentHealthChange(const FOnAttributeChangeData& Data) const
{
    OnHeroHpChanged.Broadcast(Data.NewValue);
}

void AHeroCharacter::MaxHealthChange(const FOnAttributeChangeData& Data) const
{
    OnHeroMaxHpChanged.Broadcast(Data.NewValue);
}

void AHeroCharacter::CurrentCostChange(const FOnAttributeChangeData& Data) const
{
    OnHeroCostChanged.Broadcast(Data.NewValue);
}

void AHeroCharacter::MaxCostChange(const FOnAttributeChangeData& Data) const
{
    OnHeroMaxCostChanged.Broadcast(Data.NewValue);
}
#pragma endregion

void AHeroCharacter::Tick(float DeltaSeconds)
{
    if (bIsAuto)
    {
        UE_LOG(LogTemp, Log, TEXT("bIsAuto = true"));
        AutoMode();
    }
}


#pragma region MyRegion

//void AHeroCharacter::EquipWeapon(UDataAsset_WeaponData* WeaponData)
//{
//    Weapon = WeaponData;
//
//    if(Weapon)
//    {
//        const FPGHeroWeaponData& Data = Weapon->GetHeroWeaponData();
//        if (PGAbilitySystemComponent)
//        {
//            if(Data.BaseAttackAbility)
//            {
//                   AttackHandle = PGAbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(Data.BaseAttackAbility, 1));
//            }
//            if (!(Data.WeaponSkillAbilities.IsEmpty()))
//            {
//                for (const TSubclassOf<UGameplayAbility>& ability : Data.WeaponSkillAbilities)
//                {
//                    SkillHandle.AddUnique(PGAbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(ability, 1)));
//                }
//            }
//        }
//
//        WeaponStaticMesh->SetStaticMesh(Weapon->GetHeroWeaponData().SoftWeaponMesh.Get());
//        //차후 에셋이 정해지면 소켓으로 붙일 예정
//    }
//    
//}

//void AHeroCharacter::EquipArmor(UDataAsset_ArmorData* ArmorData)
//{
//    Armor = ArmorData;
//
//    if (Armor)
//    {
//    }
//}
//
//void AHeroCharacter::EquipAccessory(UDataAsset_AccessoryData* AccessoryData)
//{
//    Accessory = AccessoryData;
//    if (Accessory)
//    {
//
//    }
//}
//
//void AHeroCharacter::UnEquipWeapon()
//{
//    if (PGAbilitySystemComponent)
//    {
//        if (AttackHandle.IsValid())
//        {
//            PGAbilitySystemComponent->ClearAbility(AttackHandle);
//            AttackHandle = FGameplayAbilitySpecHandle();
//        }
//        if (!(SkillHandle.IsEmpty()))
//        {
//            for (FGameplayAbilitySpecHandle handle : SkillHandle)
//            {
//                PGAbilitySystemComponent->ClearAbility(handle);
//                SkillHandle.RemoveSwap(handle);
//            }
//        }
//    }
//    Weapon = nullptr;
//}
//
//void AHeroCharacter::UnEquipArmor()
//{
//    Armor = nullptr;
//}
//
//void AHeroCharacter::UnEquipAccessory()
//{
//    Accessory = nullptr;
//}

void AHeroCharacter::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    UE_LOG(LogTemp, Log, TEXT("Overlap"));

    APGCharacterBase* Unit = Cast<APGCharacterBase>(OtherActor);

    if (Unit)
    {
        if (Unit->GetTeamTag().MatchesTag(PGGameplayTags::Unit_Side_Foe))
        {
            PotentialTargets.AddUnique(Unit);
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("not enemy\n"));
            UE_LOG(LogTemp, Warning, TEXT("Unit BP Tag: %s"), *Unit->GetTeamTag().ToString());
        }
    }
}

void AHeroCharacter::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    UE_LOG(LogTemp, Log, TEXT("UnOverlap"));
    APGCharacterBase* Unit = Cast<APGCharacterBase>(OtherActor);

    if (Unit)
    {
        if (Unit->GetTeamTag().MatchesTag(PGGameplayTags::Unit_Side_Foe))
        {
            PotentialTargets.RemoveSwap(Unit);
        }
    }
}
//
//void AHeroCharacter::ActivateAttack()
//{
//    AActor* AttackTarget = GetClosestTargetGetClosestTarget(PotentialTargets);
//
//    FGameplayEventData EventData;
//    EventData.Instigator = this;
//    EventData.Target = AttackTarget;
//
//    /*UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, FGameplayTag::RequestGameplayTag(FName("Player_Ability_BasicAttack_Melee")), EventData);*/
//    if (PGAbilitySystemComponent)
//    {
//        //FGameplayTagContainer attack(TAG_Player_Ability_BasicAttack);
//        //PGAbilitySystemComponent->TryActivateAbilitiesByTag(attack);
//        PGAbilitySystemComponent->TryActivateAbilityByTag(PGGameplayTags::Player_Ability_BasicAttack);
//    }
//}
//
//void AHeroCharacter::AutoBattle()
//{
//    if (PotentialTargets.IsEmpty())
//    {
//        AddMovementInput(FVector::ForwardVector);
//    }
//    else
//    {
//        AddMovementInput(FVector::ZeroVector);
//        ActivateSkill();
//    }
//}
//
//AActor* AHeroCharacter::GetClosestTarget(const TArray<AActor*>& TargetArray)
//{
//    if (TargetArray.IsEmpty())
//    {
//        return nullptr;
//    }
//
//    AActor* ClosestActor = nullptr;
//
//    float MinDistanceSq = MAX_flt;
//
//    for (AActor* Target : TargetArray)
//    {
//        if (IsValid(Target))
//        {
//            const float CurrentDistanceSq = this->GetSquaredDistanceTo(Target);
//
//            if (CurrentDistanceSq < MinDistanceSq)
//            {
//                MinDistanceSq = CurrentDistanceSq;
//                ClosestActor = Target;
//            }
//        }
//    }
//
//    return ClosestActor;
//}

//void AHeroCharacter::OnMovementInput(const FInputActionValue& InValue)
//{
//    //if (Controller)
//    //{
//    //    FVector2D InputDirection = InValue.Get<FVector2D>();
//    //    FVector MoveDirection = FVector(InputDirection.X, InputDirection.Y, 0.0f);
//    //    //UE_LOG(LogTemp, Log, TEXT("%.1f, %.1f, %.1f"), MoveDirection.X, MoveDirection.Y, MoveDirection.Z);
//    //    AddMovementInput(MoveDirection);
//    //}
//    //else
//    //    UE_LOG(LogTemp, Log, TEXT("Controller Unavailable"));
//}

//// Called every frame
//void AHeroCharacter::Tick(float DeltaTime)
//{
//    Super::Tick(DeltaTime);
//
//    if (!(PotentialTargets.IsEmpty()))
//    {
//        CurrentTarget = GetClosestTarget(PotentialTargets);
//
//        if (CurrentTarget.IsValid())
//        {
//            //CurrentTarget을 바라보도록 회전
//            FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), CurrentTarget->GetActorLocation());
//            LookAtRotation.Pitch = 0.f; // 수평 회전만 허용
//            SetActorRotation(LookAtRotation);
//        }
//
//        ActivateAttack();
//    }
//    else
//    {
//        CurrentTarget = nullptr;
//    }
//
//    if (bIsAuto)
//    {
//        AutoBattle();
//    }
//}

//void AHeroCharacter::ActivateSkill()
//{
//    if (PGAbilitySystemComponent)
//    {
//        // 주석
//        //FGameplayTagContainer Skill1(TAG_Player_Ability_Skill_1);
//        //PGAbilitySystemComponent->TryActivateAbilitiesByTag(Skill1);
//        //FGameplayTagContainer Skill2(TAG_Player_Ability_Skill_2);
//        //PGAbilitySystemComponent->TryActivateAbilitiesByTag(Skill2);
//
//        // 현재 스킬 1개로 예정
//        PGAbilitySystemComponent->TryActivateAbilityByTag(PGGameplayTags::Player_Ability_Skill);
//    }
//}
#pragma endregion

void AHeroCharacter::UnEquipWeapon()
{
    if (PGAbilitySystemComponent)
    {
        if (AttackHandle.IsValid())
        {
            PGAbilitySystemComponent->ClearAbility(AttackHandle);
            AttackHandle = FGameplayAbilitySpecHandle();
        }
        if (!(SkillHandle.IsEmpty()))
        {
            for (FGameplayAbilitySpecHandle handle : SkillHandle)
            {
                PGAbilitySystemComponent->ClearAbility(handle);
                SkillHandle.RemoveSwap(handle);
            }
        }
    }
    Weapon = nullptr;
}

void AHeroCharacter::UnEquipArmor()
{
    Armor = nullptr;
}

void AHeroCharacter::UnEquipAccessory()
{
    Accessory = nullptr;
}