// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/PGAbilitySystemGlobals.h"
#include "AbilitySystem/Types/PGGameplayEffectContext.h"

FGameplayEffectContext* UPGAbilitySystemGlobals::AllocGameplayEffectContext() const
{
    return new FPGGameplayEffectContext();
}

