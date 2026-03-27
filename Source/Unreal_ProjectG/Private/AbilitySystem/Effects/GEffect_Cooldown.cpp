// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Effects/GEffect_Cooldown.h"
#include "PGGameplayTags.h"

UGEffect_Cooldown::UGEffect_Cooldown()
{
    // DurationPolicy 정책을 결정하는 부분.
    DurationPolicy = EGameplayEffectDurationType::HasDuration;

    FSetByCallerFloat SetByCallerDuration; 
    SetByCallerDuration.DataTag = PGGameplayTags::Shared_SetByCaller_Duration;
    DurationMagnitude = FGameplayEffectModifierMagnitude(SetByCallerDuration); // DurationMagnitude는 FGameplayEffectModifierMagnitude 구조체로, SetByCallerDuration을 사용하여 설정됩니다. 이는 이 효과의 지속 시간을 런타임에 설정할 수 있도록 합니다.
}
