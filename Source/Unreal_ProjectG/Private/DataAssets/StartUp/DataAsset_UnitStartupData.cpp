// Fill out your copyright notice in the Description page of Project Settings.


#include "DataAssets/StartUp/DataAsset_UnitStartupData.h"
#include "AbilitySystem/PGAbilitySystemComponent.h"
#include "AbilitySystem/Abilities/PGUnitGameplayAbility.h"
#include "DataAssets/Ability/DataAsset_SkillData.h"

void UDataAsset_UnitStartupData::GiveToAbilitySystemComponent(UPGAbilitySystemComponent* InASCToGive, int32 InLevel)
{
    Super::GiveToAbilitySystemComponent(InASCToGive, InLevel);

    if (!UnitCombatAbilityEntries.IsEmpty())
    {
        for (UDataAsset_SkillData* SkillData : UnitCombatAbilityEntries)
        {
            if (!SkillData) continue;

            TSubclassOf<UGameplayAbility> AbilityClass = SkillData->AbilityEntry.AbilityClass.LoadSynchronous();

            FGameplayAbilitySpec AbilitySpec(
                AbilityClass,
                InLevel,
                INDEX_NONE,
                SkillData
            );

            InASCToGive->GiveAbility(AbilitySpec);
        }
    }
}


//void UDataAsset_UnitStartupData::GiveToAbilitySystemComponent(UPGAbilitySystemComponent* InASCToGive, int32 InLevel)
//{
//    Super::GiveToAbilitySystemComponent(InASCToGive, InLevel);
//
//    if (!UnitCombatAbilities.IsEmpty())
//    {
//        for (const TSubclassOf<UPGUnitGameplayAbility>& AbilityClass : UnitCombatAbilities)
//        {
//            if (!AbilityClass) continue;
//
//            FGameplayAbilitySpec AbilitySpec(AbilityClass);
//            AbilitySpec.SourceObject = InASCToGive->GetAvatarActor();
//            AbilitySpec.Level = InLevel;
//
//            InASCToGive->GiveAbility(AbilitySpec);
//        }
//    }
//}
