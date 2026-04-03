// Fill out your copyright notice in the Description page of Project Settings.


#include "DataAssets/Items/DataAsset_AccessoryData.h"
#include "AbilitySystem/Abilities/Hero/PGHeroSkillGameplayAbility.h"

bool UDataAsset_AccessoryData::IsValid() const
{
    return (AccessoryAbilityClass != nullptr && AccessoryAbilityData.IsValid()) || AccessoryAbility; // 액세서리가 어빌리티 클래스를 가지고 있고, 그에 대응하는 데이터가 유효하거나, 직접적으로 액세서리 어빌리티가 유효한 경우
}

