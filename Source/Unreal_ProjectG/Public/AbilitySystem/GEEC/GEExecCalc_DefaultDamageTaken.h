// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "GEExecCalc_DefaultDamageTaken.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_PROJECTG_API UGEExecCalc_DefaultDamageTaken : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
	
public:
    UGEExecCalc_DefaultDamageTaken();

    static float GetElementMultiplier(const FGameplayTagContainer& SourceTags, const FGameplayTagContainer& TargetTags);

    virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
