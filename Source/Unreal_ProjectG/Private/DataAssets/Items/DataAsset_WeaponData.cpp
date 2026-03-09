// Fill out your copyright notice in the Description page of Project Settings.


#include "DataAssets/Items/DataAsset_WeaponData.h"
#include "DataAssets/Ability/DataAsset_SkillData.h"
#include "AbilitySystem/PGAbilitySystemComponent.h"
#include "AbilitySystem/Abilities/PGGameplayAbility.h"

void UDataAsset_WeaponData::GrantWeaponAbilityToAbilityComponent(UPGAbilitySystemComponent* InASCToGive, int32 ApplyLevel, TArray<FGameplayAbilitySpecHandle>& OutWeaponAbilitySpecHandles)
{
    if (InASCToGive)
    {
        // 무기 기본 공격 어빌리티 부여 및 핸들 저장
        UDataAsset_SkillData* LoadedData = WeaponBasicAttackData.LoadSynchronous();
        if (LoadedData && LoadedData->AbilityEntry.AbilityClass)
        {
            FGameplayAbilitySpec BasicAttackAbilitySpec(
                LoadedData->AbilityEntry.AbilityClass,
                ApplyLevel,
                INDEX_NONE,
                LoadedData
            );
            OutWeaponAbilitySpecHandles.Add(InASCToGive->GiveAbility(BasicAttackAbilitySpec));
        }

        // 무기 스킬 어빌리티 부여 및 핸들 저장
        for(const TSoftObjectPtr<UDataAsset_SkillData>& SkillDataPtr : WeaponSkillDataArray)
        {
            UDataAsset_SkillData* LoadedSkillData = SkillDataPtr.LoadSynchronous();
            if (LoadedSkillData && LoadedSkillData->AbilityEntry.AbilityClass)
            {
                FGameplayAbilitySpec SkillAbilitySpec(
                    LoadedSkillData->AbilityEntry.AbilityClass,
                    ApplyLevel,
                    INDEX_NONE,
                    LoadedSkillData
                );
                OutWeaponAbilitySpecHandles.Add(InASCToGive->GiveAbility(SkillAbilitySpec));
            }
        }
    }
}
