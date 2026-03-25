#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/PGHeroGameplayAbility.h"
#include "DataAssets/Ability/DataAsset_HeroSkillData.h"
#include "PGHeroSkillGameplayAbility.generated.h"

UCLASS()
class UNREAL_PROJECTG_API UPGHeroSkillGameplayAbility : public UPGHeroGameplayAbility
{
    GENERATED_BODY()

public:
    UPGHeroSkillGameplayAbility();

protected:
    virtual void ActivateAbility(
        const FGameplayAbilitySpecHandle Handle,
        const FGameplayAbilityActorInfo* ActorInfo,
        const FGameplayAbilityActivationInfo ActivationInfo,
        const FGameplayEventData* TriggerEventData) override;

    virtual void EndAbility(
        const FGameplayAbilitySpecHandle Handle,
        const FGameplayAbilityActorInfo* ActorInfo,
        const FGameplayAbilityActivationInfo ActivationInfo,
        bool bReplicateEndAbility,
        bool bWasCancelled) override;

private:
    void ExecuteNextAction();

    void ExecuteEventTrigger(EHeroSkillEventTrigger Trigger, const FGameplayTag& SourceEventTag, const FGameplayAbilityTargetDataHandle& TargetData);
    void ExecuteEventAction(const FSkillActionRow& ActionRow, const FGameplayAbilityTargetDataHandle& TargetData);

    static bool ConvertEventTagToTrigger(const FGameplayTag& EventTag, EHeroSkillEventTrigger& OutTrigger);

    UFUNCTION()
    void OnTaskRuntimeEvent(FGameplayTag EventTag, FGameplayAbilityTargetDataHandle TargetData);

    UFUNCTION()
    void OnActionCompleted(FGameplayAbilityTargetDataHandle TargetData);

    UFUNCTION()
    void OnActionCancelled(FGameplayAbilityTargetDataHandle TargetData);

private:
    int32 CurrentActionIndex = 0;

    UPROPERTY()
    TObjectPtr<UDataAsset_HeroSkillData> SkillData = nullptr;

    UPROPERTY()
    TArray<FSkillActionRow> RuntimeActionSequence;

    bool bAutoMode = false;
};
