// Fill out your copyright notice in the Description page of Project Settings.
#include "AbilitySystem/MMC/MMCalculation_DefaultGiveBuff.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffectTypes.h"
#include "GameplayEffectExtension.h"
#include "GameplayEffect.h"
#include "AbilitySystem/PGCharacterAttributeSet.h"
#include "PGGameplayTags.h"

namespace DefaultGivenBuffTags
{
    static const FGameplayTag Buff_AttackPower = PGGameplayTags::Buff_AttackPower;
    static const FGameplayTag Buff_MaxHealth = PGGameplayTags::Buff_MaxHealth;

    static const FGameplayTag SetByCaller_Multiplier = PGGameplayTags::Shared_SetByCaller_DamageMultiplier;
    static const FGameplayTag SetByCaller_BaseBuffAmount = PGGameplayTags::Shared_SetByCaller_BaseBuffAmount;
}

struct FDefaultGivenBuffStatics
{
    DECLARE_ATTRIBUTE_CAPTUREDEF(AttackPower);
    DECLARE_ATTRIBUTE_CAPTUREDEF(MaxHealth);

    FDefaultGivenBuffStatics()
    {
        // Source에서 캡처
        DEFINE_ATTRIBUTE_CAPTUREDEF(UPGCharacterAttributeSet, AttackPower, Target, true);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UPGCharacterAttributeSet, MaxHealth, Target, true);
    }
};

static const FDefaultGivenBuffStatics& DefaultGivenBuffStatics()
{
    static FDefaultGivenBuffStatics Statics;
    return Statics;
}

UMMCalculation_DefaultGiveBuff::UMMCalculation_DefaultGiveBuff()
{
    RelevantAttributesToCapture.Add(DefaultGivenBuffStatics().AttackPowerDef);
    RelevantAttributesToCapture.Add(DefaultGivenBuffStatics().MaxHealthDef);
}

float UMMCalculation_DefaultGiveBuff::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
    return 0.0f;
}
