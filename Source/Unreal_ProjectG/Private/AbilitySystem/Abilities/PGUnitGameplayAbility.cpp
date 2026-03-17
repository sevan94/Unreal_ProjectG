// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/PGUnitGameplayAbility.h"
#include "AbilitySystem/PGAbilitySystemComponent.h"
#include "Character/Unit/UnitCharacter.h"
#include "PGGameplayTags.h"

AUnitCharacter* UPGUnitGameplayAbility::GetUnitCharacterFromActorInfo()
{
    if (!CachedUnitCharacter.IsValid())
    {
        CachedUnitCharacter = Cast<AUnitCharacter>(CurrentActorInfo->AvatarActor);
    }
    return CachedUnitCharacter.IsValid() ? CachedUnitCharacter.Get() : nullptr;
}
