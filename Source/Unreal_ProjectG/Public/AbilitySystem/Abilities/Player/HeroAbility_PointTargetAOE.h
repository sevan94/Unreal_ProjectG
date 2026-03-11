// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/Player/HeroAbility_AOEBase.h"
#include "HeroAbility_PointTargetAOE.generated.h"

/**
 * 플레이어가 원하는 위치를 지정하여 해당 위치를 중심으로 AOE 공격을 하는 어빌리티
 * 
 * 1. ActivateAbility -> WaitTargetData -> GATargetActor_AOEGroundTrace 스폰
 * 2. 플레이어가 위치 확정 -> OnTargetReady -> 몽타주 재생 -> WaitGameplayEvent
 * 3. Shared_Event_AOEExecute 이벤트 수신 -> OnApplyAOEDamage -> SpawnAndInitializeAOEActor
 */
UCLASS()
class UNREAL_PROJECTG_API UHeroAbility_PointTargetAOE : public UHeroAbility_AOEBase
{
	GENERATED_BODY()
	
public:
    UHeroAbility_PointTargetAOE();

protected:
    virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
    virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

private:
    UFUNCTION(BlueprintCallable)
    void OnTargetDataReady(const FGameplayAbilityTargetDataHandle& InTargetDataHandle);

    UFUNCTION(BlueprintCallable)
    void OnTargetDataCancelled(const FGameplayAbilityTargetDataHandle& InTargetDataHandle);

    UFUNCTION()
    void OnApplyAOEDamage(FGameplayEventData EventData);

    UFUNCTION()
    void OnAOEMontageFinished();

private:
    FVector CachedHitLocation;
};
