// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/AOEActor/AOEInstantEffectActor.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "PGGameplayTags.h"

void AAOEInstantEffectActor::BP_CollectAndApplyEffects(float InSkillMultiplier)
{
    for(AActor* Target : CollectValidTargets())
    {
        if (!EffectSpecHandle.IsValid()) return;
        if (!IsValid(Target)) continue;

        UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Target);
        if (!TargetASC) continue;

        EffectSpecHandle.Data->SetSetByCallerMagnitude(PGGameplayTags::Shared_SetByCaller_SkillMultiplier, InSkillMultiplier);
        TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
    }
}