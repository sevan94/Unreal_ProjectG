// Fill out your copyright notice in the Description page of Project Settings.


#include "DataAssets/Items/DataAsset_WeaponData.h"
#include "DataAssets/Ability/DataAsset_HeroSkillData.h"
#include "AbilitySystem/PGAbilitySystemComponent.h"
#include "PGGameplayTags.h"

void UDataAsset_WeaponData::GrantWeaponAbilityToAbilityComponent(UPGAbilitySystemComponent* InASCToGive, int32 ApplyLevel, TArray<FGameplayAbilitySpecHandle>& OutWeaponAbilitySpecHandles)
{
    if (InASCToGive)
    {
        BasicAttackAbility.LoadData();
        // 무기 어빌리티 부여
        if (BasicAttackAbility.IsValid())
        {
            FGameplayAbilitySpec BasicAttackAbilitySpec(
                BasicAttackAbility.AbilityClass,
                ApplyLevel);

            // SourceObject로 스킬 데이터 에셋을 전달하여 어빌리티가 해당 데이터를 참조할 수 있도록 함
            // TODO : 비동기 로드로 변경 필요 (비동기 로드 완료 후 어빌리티에 데이터 에셋 할당)
            BasicAttackAbilitySpec.SourceObject = BasicAttackAbility.AbilityData.LoadSynchronous();
            BasicAttackAbilitySpec.GetDynamicSpecSourceTags().AddTag(PGGameplayTags::Input_BasicAttack);
            
            OutWeaponAbilitySpecHandles.Add(InASCToGive->GiveAbility(BasicAttackAbilitySpec));
        }

        SubSkillAbility.LoadData();
        if(SubSkillAbility.IsValid())
        {
            FGameplayAbilitySpec SubSkillAbilitySpec(
                SubSkillAbility.AbilityClass,
                ApplyLevel);
            // SourceObject로 스킬 데이터 에셋을 전달하여 어빌리티가 해당 데이터를 참조할 수 있도록 함
            SubSkillAbilitySpec.SourceObject = SubSkillAbility.AbilityData.LoadSynchronous();
            SubSkillAbilitySpec.GetDynamicSpecSourceTags().AddTag(PGGameplayTags::Input_ActiveSkill_SubSkill);
            OutWeaponAbilitySpecHandles.Add(InASCToGive->GiveAbility(SubSkillAbilitySpec));
        }

        MainSkillAbility.LoadData();
        if(MainSkillAbility.IsValid())
        {
            FGameplayAbilitySpec MainSkillAbilitySpec(
                MainSkillAbility.AbilityClass,
                ApplyLevel);
            // SourceObject로 스킬 데이터 에셋을 전달하여 어빌리티가 해당 데이터를 참조할 수 있도록 함
            MainSkillAbilitySpec.SourceObject = MainSkillAbility.AbilityData.LoadSynchronous();
            MainSkillAbilitySpec.GetDynamicSpecSourceTags().AddTag(PGGameplayTags::Input_ActiveSkill_MainSkill);

            OutWeaponAbilitySpecHandles.Add(InASCToGive->GiveAbility(MainSkillAbilitySpec));
        }
    }
}
