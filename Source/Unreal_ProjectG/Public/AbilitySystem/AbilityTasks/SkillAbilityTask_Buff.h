// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetTypes.h"
#include "AbilitySystem/AbilityTasks/SkillAbilityTask.h"
#include "DataAssets/Ability/DataAsset_HeroSkillData.h"
#include "SkillAbilityTask_Buff.generated.h"

UCLASS()
class UNREAL_PROJECTG_API USkillAbilityTask_Buff : public USkillAbilityTask
{
    GENERATED_BODY()

public:
    static USkillAbilityTask_Buff* Create(
        UGameplayAbility* OwningAbility,
        const FSkillActionRow& ActionRow,
        const FGameplayAbilityTargetDataHandle& InTriggerTargetData = FGameplayAbilityTargetDataHandle());

    virtual void Activate() override;
    virtual void OnDestroy(bool bInOwnerFinished) override;

private:
    bool ApplyBuffOnce();
    void CompleteTask();
    void CancelTask();

    UFUNCTION()
    void OnMontageFinished();

    UFUNCTION()
    void OnMontageCancelled();

    UFUNCTION()
    void OnBuffEventReceived(FGameplayEventData Payload);

    bool ResolveCenterLocation(FVector& OutCenter) const;
    void GatherTargets(const FVector& CenterLocation, TArray<AActor*>& OutTargets) const;
    bool IsTargetAllowed(AActor* Candidate) const;
    float GetTargetHealth(const AActor* Target) const;
    void ApplyEffectsToTargets(const TArray<AActor*>& Targets) const;

private:
    FSkillActionRow ActionRowData;
    FGameplayAbilityTargetDataHandle TriggerTargetData;
    bool bWaitMontageFinish = false;
    bool bBuffApplied = false;
    bool bTaskResolved = false;
};
