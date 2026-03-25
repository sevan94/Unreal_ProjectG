// Fill out your copyright notice in the Description page of Project Settings.


#include "DataAssets/Items/DataAsset_AccessoryData.h"
#include "AbilitySystem/Abilities/Hero/PGHeroSkillGameplayAbility.h"

bool UDataAsset_AccessoryData::IsValid() const
{
    return AccessoryAbilityClass != nullptr && AccessoryAbilityData.IsValid();
}

