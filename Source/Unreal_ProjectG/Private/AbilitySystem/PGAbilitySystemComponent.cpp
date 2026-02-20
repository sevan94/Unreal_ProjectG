// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/PGAbilitySystemComponent.h"
#include "AbilitySystem/Abilities/PGHeroGameplayAbility.h"

void UPGAbilitySystemComponent::GrantHeroWeaponBasicAttackAbility(TSubclassOf<UPGHeroGameplayAbility> InBasicAttackAbility, int32 ApplyLevel, FGameplayAbilitySpecHandle& OutBasicAttackAbilitySpecHandle)
{
    if (!InBasicAttackAbility) return;

    FGameplayAbilitySpec AbilitySpec(InBasicAttackAbility);
    AbilitySpec.SourceObject = GetAvatarActor();
    AbilitySpec.Level = ApplyLevel;

    OutBasicAttackAbilitySpecHandle = GiveAbility(AbilitySpec);
}

void UPGAbilitySystemComponent::GrantHeroWeaponSkillAbilities(const TArray<TSubclassOf<UPGHeroGameplayAbility>>& InWeaponSkillAbilities, int32 ApplyLevel, TArray<FGameplayAbilitySpecHandle>& OutWeaponAbilitySpecHandles)
{
    if (InWeaponSkillAbilities.IsEmpty()) return;

    for (const TSubclassOf<UPGHeroGameplayAbility>& AbilityClass : InWeaponSkillAbilities)
    {
        if(!AbilityClass) continue;

        FGameplayAbilitySpec AbilitySpec(AbilityClass);
        AbilitySpec.SourceObject = GetAvatarActor();
        AbilitySpec.Level = ApplyLevel;

        // 부여한 어빌리티 삭제를 위해 핸들을 저장
        OutWeaponAbilitySpecHandles.AddUnique(GiveAbility(AbilitySpec));
    }
}
