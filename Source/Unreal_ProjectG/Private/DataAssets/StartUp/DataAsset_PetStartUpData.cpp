// Fill out your copyright notice in the Description page of Project Settings.


#include "DataAssets/StartUp/DataAsset_PetStartUpData.h"
#include "DataAssets/Ability/DataAsset_SkillData.h"
#include "AbilitySystem/PGAbilitySystemComponent.h"
#include "AbilitySystem/Abilities/PGGameplayAbility.h"

void UDataAsset_PetStartUpData::GiveToAbilitySystemComponent(UPGAbilitySystemComponent* InASCToGive, int32 InLevel)
{
    Super::GiveToAbilitySystemComponent(InASCToGive, InLevel);

    if (!UPetCombatAbility.IsEmpty())
    {
        for (UDataAsset_SkillData* SkillData : UPetCombatAbility)
        {
            if (!SkillData) continue;
            FGameplayAbilitySpec AbilitySpec(
                SkillData->AbilityEntry.AbilityClass,
                InLevel,
                INDEX_NONE,
                SkillData
            );
            InASCToGive->GiveAbility(AbilitySpec);
        }
    }
}
