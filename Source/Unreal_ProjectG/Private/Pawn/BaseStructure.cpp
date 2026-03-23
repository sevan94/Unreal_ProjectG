
#include "Pawn/BaseStructure.h"
#include "AbilitySystem/PGAbilitySystemComponent.h"
#include "AbilitySystem/PGCharacterAttributeSet.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameplayEffectTypes.h"
#include "Character/Unit/SubSystem/UnitSubsystem.h"
#include "PGGameplayTags.h"

ABaseStructure::ABaseStructure()
{
    PrimaryActorTick.bCanEverTick = false;


    // 3. GAS 컴포넌트 생성
    if (PGAbilitySystemComponent)
    {
        PGAbilitySystemComponent->SetIsReplicated(true);
    }
}



void ABaseStructure::BeginPlay()
{
    Super::BeginPlay();


    if (UUnitSubsystem* Subsystem = GetWorld()->GetSubsystem<UUnitSubsystem>())
    {
        Subsystem->RegisterUnit(this, TeamTag);
        Subsystem->OnBossSpawnDelegate.AddDynamic(this, &ABaseStructure::ActivateInvincible);
        Subsystem->OnBossDeadDelegate.AddDynamic(this, &ABaseStructure::DeActivateInvincible);
    }

    if (PGAbilitySystemComponent)
    {
        PGAbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
            CharacterAttributeSet->GetHealthAttribute()).AddUObject(this, &ABaseStructure::CurrentHealthChange);
        PGAbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
            CharacterAttributeSet->GetMaxHealthAttribute()).AddUObject(this, &ABaseStructure::MaxHealthChange);

        // 초기 스탯 적용 (InitStatEffect가 있다면)
        if (InitStatEffect)
        {
            FGameplayEffectContextHandle ContextHandle = PGAbilitySystemComponent->MakeEffectContext();
            ContextHandle.AddSourceObject(this);

            FGameplayEffectSpecHandle SpecHandle = PGAbilitySystemComponent->MakeOutgoingSpec(InitStatEffect, 1.0f, ContextHandle);
            UE_LOG(LogTemp, Warning, TEXT("Base Initialize Effect 적용"));

            if (SpecHandle.IsValid())
            {
                PGAbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
            }
        }

    }
}

UAbilitySystemComponent* ABaseStructure::GetAbilitySystemComponent() const
{
    return PGAbilitySystemComponent;
}

void ABaseStructure::DestroyBase()
{
    //UE_LOG(LogTemp, error, TEXT("Base Destroyed!");

    // 이미 파괴된 상태라면 무시
    if (!this->IsValidLowLevel() || IsActorBeingDestroyed()) return;


    // 1. 게임 모드에 알림 (승패 판정)
    if (OnBaseDestroyed.IsBound())
    {
        OnBaseDestroyed.Broadcast(TeamID);
    }

    // 2. 기지 파괴
    Destroy();
}

void ABaseStructure::OnDie()
{
    DestroyBase();
}


void ABaseStructure::CurrentHealthChange(const FOnAttributeChangeData& Data) const
{
    OnBaseHpChanged.Broadcast(TeamTag, Data.NewValue);
}
void ABaseStructure::MaxHealthChange(const FOnAttributeChangeData& Data) const
{
    UE_LOG(LogTemp, Warning, TEXT("MaxHealthChange BroadCast"));
    OnBaseMaxHpChanged.Broadcast(TeamTag, Data.NewValue);
}

void ABaseStructure::ActivateInvincible(FGameplayTag InTeamTag)
{
    if (InTeamTag != TeamTag)
    {
        return;
    }
    FGameplayTag InvincibleTag = PGGameplayTags::Unit_State_Invincible;
    PGAbilitySystemComponent->AddLooseGameplayTag(InvincibleTag);
    BP_OnToggleInvincibleEffect(true);
}

void ABaseStructure::DeActivateInvincible(FGameplayTag InTeamTag)
{
    if (InTeamTag != TeamTag)
    {
        return;
    }
    FGameplayTag InvincibleTag = PGGameplayTags::Unit_State_Invincible;
    PGAbilitySystemComponent->AddLooseGameplayTag(InvincibleTag);
    BP_OnToggleInvincibleEffect(false);

}


