// Fill out your copyright notice in the Description page of Project Settings.


#include "DataAssets/Items/DataAsset_WeaponData.h"
#include "DataAssets/Ability/DataAsset_HeroSkillData.h"
#include "AbilitySystem/PGAbilitySystemComponent.h"
#include "PGGameplayTags.h"

void UDataAsset_WeaponData::GrantWeaponAbilityToAbilityComponent(UPGAbilitySystemComponent* InASCToGive, int32 ApplyLevel, TArray<FGameplayAbilitySpecHandle>& OutWeaponAbilitySpecHandles)
{
    if (InASCToGive)
    {
        // 무기 어빌리티 부여
        if (BasicAttackAbility.IsValid())
        {
            FGameplayAbilitySpec BasicAttackAbilitySpec(
                BasicAttackAbility.AbilityClass,
                ApplyLevel);

            // SourceObject로 스킬 데이터 에셋을 전달하여 어빌리티가 해당 데이터를 참조할 수 있도록 함
            BasicAttackAbilitySpec.SourceObject = BasicAttackAbility.AbilityData.LoadSynchronous();
            BasicAttackAbilitySpec.GetDynamicSpecSourceTags().AddTag(PGGameplayTags::Input_BasicAttack);
            
            OutWeaponAbilitySpecHandles.Add(InASCToGive->GiveAbility(BasicAttackAbilitySpec));
        }

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

        //// 무기 기본 공격 어빌리티 부여 및 핸들 저장
        //UDataAsset_SkillData* LoadedData = WeaponBasicAttackData.LoadSynchronous();
        //if (LoadedData && LoadedData->AbilityEntry.AbilityClass)
        //{
        //    FGameplayAbilitySpec BasicAttackAbilitySpec(
        //        LoadedData->AbilityEntry.AbilityClass,
        //        ApplyLevel,
        //        INDEX_NONE,
        //        LoadedData
        //    );
        //    OutWeaponAbilitySpecHandles.Add(InASCToGive->GiveAbility(BasicAttackAbilitySpec));
        //}


        //// 무기 스킬 어빌리티 부여 및 핸들 저장
        //for(const TSoftObjectPtr<UDataAsset_SkillData>& SkillDataPtr : WeaponSkillDataArray)
        //{
        //    UDataAsset_SkillData* LoadedSkillData = SkillDataPtr.LoadSynchronous();
        //    const FAbilityEntry& AbilityEntryToGrant = LoadedSkillData ? LoadedSkillData->GetGivenAbilityEntryForASC(InASCToGive) : FAbilityEntry();
        //    if (LoadedSkillData && AbilityEntryToGrant.AbilityClass)
        //    {
        //        FGameplayAbilitySpec SkillAbilitySpec(
        //            AbilityEntryToGrant.AbilityClass,
        //            ApplyLevel,
        //            INDEX_NONE,
        //            LoadedSkillData
        //        );
        //        OutWeaponAbilitySpecHandles.Add(InASCToGive->GiveAbility(SkillAbilitySpec));
        //    }
        //}
    }
}
