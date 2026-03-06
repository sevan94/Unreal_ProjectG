// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/Effects/GEffect_SetByCallerAdditive.h"
#include "PGFunctionLibrary.h"

UGEffect_SetByCallerAdditive::UGEffect_SetByCallerAdditive()
{
    DurationPolicy = EGameplayEffectDurationType::Infinite;

    // GetAttributeTagMap()을 이용하여 SetByCaller Modifier 슬롯 생성
    for (const auto& Pair : UPGFunctionLibrary::GetAttributeTagMap())
    {
        FGameplayModifierInfo ModInfo;
        ModInfo.Attribute = Pair.Key;
        ModInfo.ModifierOp = EGameplayModOp::Additive;

        FSetByCallerFloat SetByCaller;
        SetByCaller.DataTag = Pair.Value;
        ModInfo.ModifierMagnitude = FGameplayEffectModifierMagnitude(SetByCaller);

        Modifiers.Add(ModInfo);
    }
}
    

