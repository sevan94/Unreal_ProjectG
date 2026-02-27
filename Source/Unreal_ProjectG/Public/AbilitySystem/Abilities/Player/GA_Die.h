// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/PGHeroGameplayAbility.h"
#include "GA_Die.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_PROJECTG_API UGA_Die : public UPGHeroGameplayAbility
{
	GENERATED_BODY()
	
public:

    UGA_Die();

    virtual void ActivateAbility(
        const FGameplayAbilitySpecHandle Handle,
        const FGameplayAbilityActorInfo* ActorInfo,
        const FGameplayAbilityActivationInfo ActivationInfo,
        const FGameplayEventData* TriggerEventData) override;

protected:
    UFUNCTION()
    void RespawnHero();

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
    float RespawnTime = 10.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability")
    TSubclassOf<class UGameplayEffect> FullHealth = nullptr;
};
