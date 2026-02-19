// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GA_InitializeHero.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_PROJECTG_API UGA_InitializeHero : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
    UGA_InitializeHero();

    virtual void ActivateAbility(
        const FGameplayAbilitySpecHandle Handle,
        const FGameplayAbilityActorInfo* ActorInfo,
        const FGameplayAbilityActivationInfo ActivationInfo,
        const FGameplayEventData* TriggerEventData) override;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
    TSubclassOf<class UGameplayEffect> FillCost = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
    TSubclassOf<UGameplayEffect> FullHeal = nullptr;

};
