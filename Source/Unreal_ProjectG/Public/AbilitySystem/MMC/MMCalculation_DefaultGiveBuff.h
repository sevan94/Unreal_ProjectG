// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MMCalculation_DefaultGiveBuff.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_PROJECTG_API UMMCalculation_DefaultGiveBuff : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()
	
public:
    UMMCalculation_DefaultGiveBuff();

    virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;
};
