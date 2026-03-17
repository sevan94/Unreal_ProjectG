// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/PGUnitGameplayAbility.h"
#include "UnitAbility_InstantAOE.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_PROJECTG_API UUnitAbility_InstantAOE : public UPGUnitGameplayAbility
{
	GENERATED_BODY()
	
public:
    UUnitAbility_InstantAOE();

protected:
    virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
    virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
    
    // AOE 액터 스폰
    UFUNCTION(BlueprintCallable)
    AAOESkillActor* SpawnAndInitializeAOEActor(const FVector& SpawnLocation, const FRotator& SpawnRotation = FRotator::ZeroRotator);
};
