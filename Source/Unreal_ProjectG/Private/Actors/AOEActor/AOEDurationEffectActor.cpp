// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/AOEActor/AOEDurationEffectActor.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "PGGameplayTags.h"

void AAOEDurationEffectActor::BP_CollectAndApplyDurationEffects(float InSkillMultiplier, float InBaseAmount, float InDuration)
{
    for (AActor* Target : CollectValidTargets())
    {
        if (!EffectSpecHandle.IsValid()) return;
        if (!IsValid(Target)) continue;

        UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Target);
        if (!TargetASC) continue;

        EffectSpecHandle.Data->SetSetByCallerMagnitude(PGGameplayTags::Shared_SetByCaller_SkillMultiplier, InSkillMultiplier);
        EffectSpecHandle.Data->SetSetByCallerMagnitude(PGGameplayTags::Shared_SetByCaller_BaseAmount, InBaseAmount);
        EffectSpecHandle.Data->SetSetByCallerMagnitude(PGGameplayTags::Shared_SetByCaller_Duration, InDuration);
        TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
    }
}