// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/Player/HeroAbility_AOEBase.h"
#include "HeroAbility_InstantAOE.generated.h"

/**
 * 즉시 발동하는 AOE 스킬의 기본 클래스, 스폰 위치는 스폰된 액터가 Offset으로 조정
 */
UCLASS()
class UNREAL_PROJECTG_API UHeroAbility_InstantAOE : public UHeroAbility_AOEBase
{
	GENERATED_BODY()
	
public:
    UHeroAbility_InstantAOE();

protected:
    virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
    virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

    UFUNCTION(BlueprintNativeEvent)
    FVector GetInstantAOESpawnLocation() const;
    virtual FVector GetInstantAOESpawnLocation_Implementation() const;

    UFUNCTION(BlueprintNativeEvent)
    FRotator GetInstantAOESpawnRotation() const;
    virtual FRotator GetInstantAOESpawnRotation_Implementation() const;

private:
    UFUNCTION()
    void OnAOEExecuteEventReceived(FGameplayEventData EventData);

    UFUNCTION()
    void OnMontageFinished();
};
