// Fill out your copyright notice in the Description page of Project Settings.


#include "DataAssets/StartUp/DataAsset_StartupDataBase.h"
#include "AbilitySystem/Abilities/PGGameplayAbility.h"
#include "AbilitySystem/PGAbilitySystemComponent.h"

void UDataAsset_StartupDataBase::GiveToAbilitySystemComponent(UPGAbilitySystemComponent* InASCToGive, int32 InLevel)
{
    check(InASCToGive);

    if(!StartUpAbilities.IsEmpty())
    {
        GrantAbilities(StartUpAbilities, InASCToGive, InLevel);
    }

    if (!StartUpGameplayEffects.IsEmpty())
    {
        for(const TSubclassOf<UGameplayEffect>& EffectClass : StartUpGameplayEffects)
        {
            if (!EffectClass) continue;

            UGameplayEffect* EffectCOO = EffectClass->GetDefaultObject<UGameplayEffect>();

            InASCToGive->ApplyGameplayEffectToSelf(EffectCOO, InLevel, InASCToGive->MakeEffectContext());
        }
    }
}

void UDataAsset_StartupDataBase::GrantAbilities(const TArray<TSubclassOf<UPGGameplayAbility>>& InAbilitiesToGive, UPGAbilitySystemComponent* InASCToGive, int32 ApplyLevel)
{
    if(InAbilitiesToGive.IsEmpty())
    {
        return;
    }

    for(const TSubclassOf<UPGGameplayAbility>& Ability : InAbilitiesToGive)
    {
        if (!Ability) continue;

        FGameplayAbilitySpec AbilitySpec(Ability);
        AbilitySpec.SourceObject = this;
        AbilitySpec.Level = ApplyLevel;

        InASCToGive->GiveAbility(AbilitySpec);
    }
}
