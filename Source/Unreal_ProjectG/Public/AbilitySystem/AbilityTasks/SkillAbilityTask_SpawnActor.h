#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetTypes.h"
#include "AbilitySystem/AbilityTasks/SkillAbilityTask.h"
#include "DataAssets/Ability/DataAsset_HeroSkillData.h"
#include "SkillAbilityTask_SpawnActor.generated.h"

class ASkillActor;

UCLASS()
class UNREAL_PROJECTG_API USkillAbilityTask_SpawnActor : public USkillAbilityTask
{
    GENERATED_BODY()

public:
    static USkillAbilityTask_SpawnActor* Create(
        UGameplayAbility* OwningAbility,
        const FSkillActionRow& ActionRow,
        bool bIsAutoMode,
        const FGameplayAbilityTargetDataHandle& InTriggerTargetData = FGameplayAbilityTargetDataHandle(),
        bool bInEmitRuntimeEvent = true);

    virtual void Activate() override;
    virtual void OnDestroy(bool bInOwnerFinished) override;

private:
    void StartWaitTargetData();

    UFUNCTION()
    void OnTargetDataReady(const FGameplayAbilityTargetDataHandle& TargetDataHandle);

    UFUNCTION()
    void OnTargetDataCancelled(const FGameplayAbilityTargetDataHandle& TargetDataHandle);

    FVector ResolveAutoTargetLocation() const;
    bool TryResolveSpawnFromTriggerData(FVector& OutLocation, FRotator& OutRotation) const;

    void PlayMontageOrSpawn();

    UFUNCTION()
    void OnMontageFinished();

    UFUNCTION()
    void OnMontageCancelled();

    UFUNCTION()
    void OnSpawnEventReceived(FGameplayEventData Payload);

    void SpawnActorAtLocation(const FVector& Location, const FRotator& Rotation);

private:
    FSkillActionRow CachedActionRow;
    bool bAutoMode = false;
    bool bEmitRuntimeEvent = true;
    bool bWaitMontageFinish = false;
    bool bActorSpawned = false;
    bool bTaskResolved = false;

    FGameplayAbilityTargetDataHandle TriggerTargetData;

    FVector CachedSpawnLocation = FVector::ZeroVector;
    FRotator CachedSpawnRotation = FRotator::ZeroRotator;
};
