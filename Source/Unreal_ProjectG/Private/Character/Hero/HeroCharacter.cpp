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
}

UPawnCombatComponent* AHeroCharacter::GetPawnCombatComponent() const
{
    return HeroCombatComponent;
}

void AHeroCharacter::SpawnCharacter()
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
    MovementComponent->DisableMovement();
    MovementComponent->StopMovementImmediately();
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

void AHeroCharacter::BroadCastAttributeSet()
{
    if (ResourceAttribute)
    {
        PGAbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
            ResourceAttribute->GetHealthAttribute()).AddUObject(this, &AHeroCharacter::CurrentHealthChange);
        PGAbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
            ResourceAttribute->GetMaxHealthAttribute()).AddUObject(this, &AHeroCharacter::MaxHealthChange);
        PGAbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
            ResourceAttribute->GetCostAttribute()).AddUObject(this, &AHeroCharacter::CurrentCostChange);
        PGAbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
            ResourceAttribute->GetMaxCostAttribute()).AddUObject(this, &AHeroCharacter::MaxCostChange);
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
        if(UDataAsset_StartupDataBase* LoadData = CharacterStartupData.LoadSynchronous())
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
    }

    BroadCastAttributeSet();
}

// Called every frame
void AHeroCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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

void AHeroCharacter::OnMovementInput(const FInputActionValue& InValue)
{
    if (Controller)
    {
        FVector2D InputDirection = InValue.Get<FVector2D>();
        FVector MoveDirection = FVector(InputDirection.X, InputDirection.Y, 0.0f);
        //UE_LOG(LogTemp, Log, TEXT("%.1f, %.1f, %.1f"), MoveDirection.X, MoveDirection.Y, MoveDirection.Z);
        AddMovementInput(MoveDirection);
    }
    else
        UE_LOG(LogTemp, Log, TEXT("Controller Unavailable"));
}

void AHeroCharacter::OnAttackInput()
{

}

