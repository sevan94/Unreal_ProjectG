// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Hero/HeroCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "Components/Resource/HeroResourceComponent.h"
#include "Components/CapsuleComponent.h"
#include "UI/Battle/ControlPanelWidget.h"
#include "Components/Combat/HeroCombatComponent.h"
#include "DataAssets/StartUp/DataAsset_HeroStartupData.h"
#include "AbilitySystem/PGCharacterAttributeSet.h"
#include "AbilitySystem/PGAbilitySystemComponent.h"
#include "Components/SphereComponent.h"
#include "Character/Unit/UnitCharacter.h"
#include "PGGameplayTags.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "DataAssets/Items/DataAsset_WeaponData.h"
#include "DataAssets/Items/DataAsset_ArmorData.h"
#include "DataAssets/Items/DataAsset_AccessoryData.h"
#include "AbilitySystem/Abilities/PGHeroGameplayAbility.h"

UE_DEFINE_GAMEPLAY_TAG(TAG_Player_Ability_Skill_1, "Player.Ability.Skill.1");
UE_DEFINE_GAMEPLAY_TAG(TAG_Player_Ability_Skill_2, "Player.Ability.Skill.2");
UE_DEFINE_GAMEPLAY_TAG(TAG_Player_Ability_BasicAttack, "Player.Ability.BasicAttack");
UE_DEFINE_GAMEPLAY_TAG(TAG_Unit_Side_Foe, "Unit.Side.Foe");

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

void AHeroCharacter::InitializeHero()
{
    if (PGAbilitySystemComponent && GA_Initialize)
    {
        PGAbilitySystemComponent->TryActivateAbilityByClass(GA_Initialize);
    }
}

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

void AHeroCharacter::EquipArmor(UDataAsset_ArmorData* ArmorData)
{
    Armor = ArmorData;

    if (Armor)
    {
    }
}

void AHeroCharacter::EquipAccessory(UDataAsset_AccessoryData* AccessoryData)
{
    Accessory = AccessoryData;
    if (Accessory)
    {

    }
}

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

void AHeroCharacter::ActivateSkill()
{
    if (PGAbilitySystemComponent)
    {
        FGameplayTagContainer Skill1(TAG_Player_Ability_Skill_1);
        PGAbilitySystemComponent->TryActivateAbilitiesByTag(Skill1);
        FGameplayTagContainer Skill2(TAG_Player_Ability_Skill_2);
        PGAbilitySystemComponent->TryActivateAbilitiesByTag(Skill2);
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
        //if (Weapon)
        //{
        //    //PGAbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(Weapon->GetHeroWeaponData()->BaseAttackAbility, 1, 1, this));
        //}
        if (GA_Initialize)
        {
            PGAbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(GA_Initialize, 1, 2, this));
        }
    }

    if (AggroCollision)
    {
        AggroCollision->OnComponentBeginOverlap.AddDynamic(this, &AHeroCharacter::OnOverlapBegin);
        AggroCollision->OnComponentEndOverlap.AddDynamic(this, &AHeroCharacter::OnOverlapEnd);
        UE_LOG(LogTemp, Log, TEXT("Overlap bind"));
    }

    BroadCastAttributeSet();
}

// Called every frame
void AHeroCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (!(PotentialTargets.IsEmpty()))
    {
        ActivateAttack();
    }

    if (bIsAuto)
    {
        AutoBattle();
    }
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
        if (Unit->GetTeamTag().MatchesTag(TAG_Unit_Side_Foe))
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
    AUnitCharacter* Unit = Cast<AUnitCharacter>(OtherActor);

    if (Unit)
    {
        if (Unit->GetTeamTag().MatchesTag(TAG_Unit_Side_Foe))
        {
            PotentialTargets.RemoveSwap(Unit);
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
    if(PGAbilitySystemComponent)
    {
        FGameplayTagContainer attack(TAG_Player_Ability_BasicAttack);
        PGAbilitySystemComponent->TryActivateAbilitiesByTag(attack);
    }
}

void AHeroCharacter::AutoBattle()
{
    if (PotentialTargets.IsEmpty())
    {
        AddMovementInput(FVector::ForwardVector);
    }
    else
    {
        AddMovementInput(FVector::ZeroVector);
        ActivateSkill();
    }
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