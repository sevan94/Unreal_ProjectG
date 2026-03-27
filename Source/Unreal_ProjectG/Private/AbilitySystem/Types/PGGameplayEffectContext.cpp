// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Types/PGGameplayEffectContext.h"

UScriptStruct* FPGGameplayEffectContext::GetScriptStruct() const
{
    return StaticStruct();
}

FGameplayEffectContext* FPGGameplayEffectContext::Duplicate() const
{
    FPGGameplayEffectContext* NewContext = new FPGGameplayEffectContext();
    *NewContext = *this;

    if (GetHitResult())
    {
        NewContext->AddHitResult(*GetHitResult(), true);
    }

    return NewContext;
}

bool FPGGameplayEffectContext::NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess)
{
    const bool bSuperResult = FGameplayEffectContext::NetSerialize(Ar, Map, bOutSuccess);

    uint8 PolarityValue = static_cast<uint8>(Polarity);
    uint8 TargetPolicyValue = static_cast<uint8>(TargetPolicy);

    Ar.SerializeBits(&PolarityValue, 8);
    Ar.SerializeBits(&TargetPolicyValue, 8);

    const bool bTagResult = EffectTags.NetSerialize(Ar, Map, bOutSuccess);

    if (Ar.IsLoading())
    {
        Polarity = (PolarityValue <= static_cast<uint8>(EPolarity::Debuff))
            ? static_cast<EPolarity>(PolarityValue)
            : EPolarity::Buff;

        TargetPolicy = (TargetPolicyValue <= static_cast<uint8>(ESkillTargetPolicy::Self))
            ? static_cast<ESkillTargetPolicy>(TargetPolicyValue)
            : ESkillTargetPolicy::Self;
    }

    bOutSuccess = bOutSuccess && bSuperResult && bTagResult;
    return true;
}
