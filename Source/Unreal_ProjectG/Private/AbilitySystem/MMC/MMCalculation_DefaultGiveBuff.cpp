// Fill out your copyright notice in the Description page of Project Settings.
#include "AbilitySystem/MMC/MMCalculation_DefaultGiveBuff.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/PGCharacterAttributeSet.h"
#include "PGGameplayTags.h"
#include "GameplayEffectExecutionCalculation.h"

struct FDefaultGivenBuffStatics
{
    DECLARE_ATTRIBUTE_CAPTUREDEF(AttackPower);

    FDefaultGivenBuffStatics()
    {
        // Source에서 캡처
        DEFINE_ATTRIBUTE_CAPTUREDEF(UPGCharacterAttributeSet, AttackPower, Source, true);
    }
};

static const FDefaultGivenBuffStatics& DefaultGivenBuffStatics()
{
    static FDefaultGivenBuffStatics StaticData;
    return StaticData;
}

UMMCalculation_DefaultGiveBuff::UMMCalculation_DefaultGiveBuff()
{
    RelevantAttributesToCapture.Add(DefaultGivenBuffStatics().AttackPowerDef);
}

float UMMCalculation_DefaultGiveBuff::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
    const float Multiplier = Spec.GetSetByCallerMagnitude(PGGameplayTags::Shared_SetByCaller_DamageMultiplier, true, 1.0f);

    const float BaseBuffAmount = Spec.GetSetByCallerMagnitude(PGGameplayTags::Shared_SetByCaller_BaseBuffAmount, true, 0.0f);
    
    float SourceAttackPower = 0.f;
    FAggregatorEvaluateParameters EvaluationParameters;
    GetCapturedAttributeMagnitude(DefaultGivenBuffStatics().AttackPowerDef, Spec, EvaluationParameters, SourceAttackPower);

    UE_LOG(LogTemp, Log, TEXT("CalculateBaseMagnitude_Implementation: Multiplier=%f, BaseBuffAmount=%f, SourceAttackPower=%f"), Multiplier, BaseBuffAmount, SourceAttackPower);

    // BaseBuffAmount + (SourceAttackPower * Multiplier) 공식으로 버프량 계산
    const float FinalBuffAmount = BaseBuffAmount + (SourceAttackPower * Multiplier);

    UE_LOG(LogTemp, Log, TEXT("FinalBuffAmount calculated: %f"), FinalBuffAmount);
    
    return FinalBuffAmount;
}
