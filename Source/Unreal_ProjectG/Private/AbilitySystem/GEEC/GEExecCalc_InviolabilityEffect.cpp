// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/GEEC/GEExecCalc_InviolabilityEffect.h"
#include "AbilitySystemComponent.h"
#include "DataAssets/Items/AccessoryDataAsset.h"

UGEExecCalc_InviolabilityEffect::UGEExecCalc_InviolabilityEffect()
{
}

void UGEExecCalc_InviolabilityEffect::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& ExecutionOutput) const
{
    UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();
    if (!TargetASC) return;

    if (AccessoryData && AccessoryData->inviolabilityEffects)
    {
        FGameplayEffectContextHandle Context = TargetASC->MakeEffectContext();
        FGameplayEffectSpecHandle SpecHandle = TargetASC->MakeOutgoingSpec(AccessoryData->inviolabilityEffects, 1.0f, Context);

        if (SpecHandle.IsValid())
        {
            TargetASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
        }
    }
}
