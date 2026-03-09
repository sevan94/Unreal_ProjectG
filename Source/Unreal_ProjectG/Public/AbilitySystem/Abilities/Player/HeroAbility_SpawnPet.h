// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/PGHeroGameplayAbility.h"
#include "HeroAbility_SpawnPet.generated.h"

class APetCharacter;
/**
 * 
 */
UCLASS()
class UNREAL_PROJECTG_API UHeroAbility_SpawnPet : public UPGHeroGameplayAbility
{
	GENERATED_BODY()

public:
    virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
    virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
    virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability|SpawnPet")
    FHeroSpawnPetAbilityConfig HeroSpawnPetConfig;
};
