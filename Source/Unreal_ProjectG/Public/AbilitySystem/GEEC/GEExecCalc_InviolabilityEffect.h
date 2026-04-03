// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "GEExecCalc_InviolabilityEffect.generated.h"
class UAccessoryDataAsset;
/**
 * 
 */
UCLASS()
class UNREAL_PROJECTG_API UGEExecCalc_InviolabilityEffect : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
public:
    UGEExecCalc_InviolabilityEffect();

    virtual void Execute_Implementation(
        const FGameplayEffectCustomExecutionParameters& ExecutionParams, 
        FGameplayEffectCustomExecutionOutput& ExecutionOutput) const override;

    UPROPERTY(EditDefaultsOnly, Category = "Effect Data")
    UAccessoryDataAsset* AccessoryData;
};
