// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemGlobals.h"
#include "PGAbilitySystemGlobals.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_PROJECTG_API UPGAbilitySystemGlobals : public UAbilitySystemGlobals
{
	GENERATED_BODY()

public:
    virtual FGameplayEffectContext* AllocGameplayEffectContext() const override;
};
