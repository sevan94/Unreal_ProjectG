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
    // FGameplayEffectContext의 NetSerialize를 먼저 호출하여 기본 필드들을 직렬화/역직렬화합니다.
    const bool bSuperResult = FGameplayEffectContext::NetSerialize(Ar, Map, bOutSuccess);

    // FPGGameplayEffectContext custom payload: cue variant tag.
    bool bTagSerializeSuccess = true;
    CueVariantTag.NetSerialize(Ar, Map, bTagSerializeSuccess);

    // 두 직렬화 결과를 모두 고려하여 최종 성공 여부를 결정합니다.
    bOutSuccess = bOutSuccess && bSuperResult && bTagSerializeSuccess;
    return true;
}
