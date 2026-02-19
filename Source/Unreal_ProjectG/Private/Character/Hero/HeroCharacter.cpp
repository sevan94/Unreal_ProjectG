// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Hero/HeroCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "Components/Resource/HeroResourceComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/Equipment/EquipmentComponent.h"
#include "UI/Battle/ControlPanelWidget.h"
#include "Components/Combat/HeroCombatComponent.h"
#include "DataAssets/StartUp/DataAsset_HeroStartupData.h"
#include "AbilitySystem/PGCharacterAttributeSet.h"
#include "AbilitySystem/PGAbilitySystemComponent.h"
#include "Components/SphereComponent.h"
#include "Character/Unit/UnitCharacter.h"
#include "PGGameplayTags.h"
#include "AbilitySystemBlueprintLibrary.h"

// Sets default values
AHeroCharacter::AHeroCharacter()
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

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
    ResourceAttribute = CreateDefaultSubobject<UPGCharacterAttributeSet>(TEXT("ResourceAttribute"));

    HeroCombatComponent = CreateDefaultSubobject<UHeroCombatComponent>(TEXT("HeroCombatComponent"));
    ResourceManager = CreateDefaultSubobject<UHeroResourceComponent>(TEXT("ResourceManager"));
    EquipmentComponent = CreateDefaultSubobject<UEquipmentComponent>(TEXT("EquipmentComponent"));

    AggroCollision = CreateDefaultSubobject<USphereComponent>(TEXT("AggroCollision"));
    AggroCollision->SetupAttachment(RootComponent);
    AggroCollision->SetSphereRadius(500.f);
    AggroCollision->SetGenerateOverlapEvents(true);
}

UPawnCombatComponent* AHeroCharacter::GetPawnCombatComponent() const
{
    return HeroCombatComponent;
}

void AHeroCharacter::SpawnHero()
{
    USkeletalMeshComponent* MeshComp = GetMesh();
    MeshComp->bPauseAnims = false;
    MeshComp->SetSimulatePhysics(false);
    MeshComp->SetCollisionProfileName(TEXT("CharacterMesh"));

    MeshComp->AttachToComponent(GetCapsuleComponent(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
    MeshComp->SetRelativeLocation(FVector(0.f, 0.f, -90.f));
    MeshComp->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

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

void AHeroCharacter::OnDie()
{
    if (PGAbilitySystemComponent && GA_Die)
    {
        PGAbilitySystemComponent->TryActivateAbilityByClass(GA_Die);
    }
    else
        UE_LOG(LogTemp, Warning, TEXT("AbilitySystem Unavailable"));
}

// Called when the game starts or when spawned
void AHeroCharacter::BeginPlay()
{
    Super::BeginPlay();

    //ABP 가져오기
    AnimInstance = GetMesh()->GetAnimInstance();

    if (!CharacterStartupData.IsNull())
    {
        if (UDataAsset_StartupDataBase* LoadData = CharacterStartupData.LoadSynchronous())
        {
            LoadData->GiveToAbilitySystemComponent(PGAbilitySystemComponent);
        }
    }

    if (PGAbilitySystemComponent)
    {
        if (GA_Die)
        {
            PGAbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(GA_Die, 1, 0, this));
        }
        if (GA_Attack)
        {
            PGAbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(GA_Attack, 1, 1, this));
        }
    }

    if (AggroCollision)
    {
        AggroCollision->OnComponentBeginOverlap.AddDynamic(this, &AHeroCharacter::OnOverlapBegin);
        UE_LOG(LogTemp, Log, TEXT("Overlap bind"));
    }
}

// Called every frame
void AHeroCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (bIsMoving)
    {
        CharacterMove();
    }

    //// 조이스틱 위젯이 있고, 입력값이 있다면 이동 처리
    //if (JoystickWidget)
    //{
    //    FVector2D JoyInput = JoystickWidget->GetJoystickVector();

    //    if (!JoyInput.IsNearlyZero())
    //    {
    //        const FVector ForwardDirection = FVector::ForwardVector;
    //        const FVector RightDirection = FVector::RightVector;

    //        // 위젯 좌표계와 월드 좌표계 매칭 (상황에 따라 Y축 반전 필요할 수 있음)
    //        AddMovementInput(ForwardDirection, JoyInput.X);
    //        AddMovementInput(RightDirection, JoyInput.Y);

    //        //UE_LOG(LogTemp, Log, TEXT("JoyInput: X=%.2f, Y=%.2f"), JoyInput.X, JoyInput.Y);
    //    }
    //}
}

// Called to bind functionality to input
void AHeroCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    UEnhancedInputComponent* enhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
    if (enhancedInputComponent)
    {
        enhancedInputComponent->BindAction(IA_Move, ETriggerEvent::Triggered, this, &AHeroCharacter::OnMovementInput);
        enhancedInputComponent->BindAction(IA_Attack, ETriggerEvent::Triggered, this, &AHeroCharacter::OnAttackInput);
    }
}

void AHeroCharacter::OnMovementInput(const FInputActionValue& InValue)
{
    //if (Controller)
    //{
    //    FVector2D InputDirection = InValue.Get<FVector2D>();
    //    FVector MoveDirection = FVector(InputDirection.X, InputDirection.Y, 0.0f);
    //    //UE_LOG(LogTemp, Log, TEXT("%.1f, %.1f, %.1f"), MoveDirection.X, MoveDirection.Y, MoveDirection.Z);
    //    AddMovementInput(MoveDirection);
    //}
    //else
    //    UE_LOG(LogTemp, Log, TEXT("Controller Unavailable"));
}

void AHeroCharacter::OnAttackInput()
{
    if (PGAbilitySystemComponent)
    {
        PGAbilitySystemComponent->TryActivateAbilityByClass(GA_Attack);
    }
}

void AHeroCharacter::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    UE_LOG(LogTemp, Log, TEXT("Overlap"));

    AUnitCharacter* Unit = Cast<AUnitCharacter>(OtherActor);

    if (Unit)
    {
        if (Unit->GetTeamTag() == PGGameplayTags::Unit_Side_Foe)
        {
            PotentialTargets.AddUnique(Unit);

            ActivateAttack();
        }
    }
}

void AHeroCharacter::ActivateAttack()
{
    AActor* AttackTarget = GetClosestTarget(PotentialTargets);

    FGameplayEventData EventData;
    EventData.Instigator = this;
    EventData.Target = AttackTarget;

    /*UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, FGameplayTag::RequestGameplayTag(FName("Player_Ability_BasicAttack_Melee")), EventData);*/
    PGAbilitySystemComponent->TryActivateAbilityByClass(GA_Attack);
}

AActor* AHeroCharacter::GetClosestTarget(const TArray<AActor*>& TargetArray)
{
    if (TargetArray.IsEmpty())
    {
        return nullptr;
    }

    AActor* ClosestActor = nullptr;

    float MinDistanceSq = MAX_flt;

    for (AActor* Target : TargetArray)
    {
        if (IsValid(Target))
        {
            const float CurrentDistanceSq = this->GetSquaredDistanceTo(Target);

            if (CurrentDistanceSq < MinDistanceSq)
            {
                MinDistanceSq = CurrentDistanceSq;
                ClosestActor = Target;
            }
        }
    }

    return ClosestActor;
}

void AHeroCharacter::MoveStart_Implementation(FVector2D JoyInput)
{
    bIsMoving = true;

    MoveDirection = FVector(JoyInput.X, JoyInput.Y, 0);
}

void AHeroCharacter::ChangeDirection_Implementation(FVector2D JoyInput)
{
    MoveDirection = FVector(JoyInput.X, JoyInput.Y, 0);
}

void AHeroCharacter::EndMovement_Implementation()
{
    bIsMoving = false;
}

void AHeroCharacter::CharacterMove()
{
    AddMovementInput(MoveDirection);
}
