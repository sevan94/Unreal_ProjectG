// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/PGAbilitySystemComponent.h"
#include "AbilitySystem/Abilities/PGHeroGameplayAbility.h"

//void UPGAbilitySystemComponent::GrantHeroWeaponBasicAttackAbility(TSubclassOf<UPGHeroGameplayAbility> InBasicAttackAbility, int32 ApplyLevel, FGameplayAbilitySpecHandle& OutBasicAttackAbilitySpecHandle)
//{
//    if (!InBasicAttackAbility) return;
//
//    FGameplayAbilitySpec AbilitySpec(InBasicAttackAbility);
//    AbilitySpec.SourceObject = GetAvatarActor();
//    AbilitySpec.Level = ApplyLevel;
//
//    OutBasicAttackAbilitySpecHandle = GiveAbility(AbilitySpec);
//}

void UPGAbilitySystemComponent::GrantHeroWeaponBasicAttackAbility(FAbilityEntry InBasicAttackAbilityEntry, int32 ApplyLevel, FGameplayAbilitySpecHandle& OutBasicAttackAbilitySpecHandle)
{
    FGameplayAbilitySpec BasicAttackAbilitySpec(
        InBasicAttackAbilityEntry.AbilityClass.Get(),
        ApplyLevel,
        INDEX_NONE,
        InBasicAttackAbilityEntry.AbilityConfig.Get()
    );
    OutBasicAttackAbilitySpecHandle = GiveAbility(BasicAttackAbilitySpec);

    UE_LOG(LogTemp, Warning, TEXT("SourceName : %s"), *BasicAttackAbilitySpec.SourceObject->GetName());
}

void UPGAbilitySystemComponent::GrantHeroWeaponSkillAbilities(const TArray<FAbilityEntry>& InWeaponSkillAbilityEntries, int32 ApplyLevel, TArray<FGameplayAbilitySpecHandle>& OutWeaponAbilitySpecHandles)
{
    if (InWeaponSkillAbilityEntries.IsEmpty()) return;

    for (const FAbilityEntry& AbilityEntry : InWeaponSkillAbilityEntries)
    {
        if(!AbilityEntry.AbilityClass) continue;

        FGameplayAbilitySpec AbilitySpec(
            AbilityEntry.AbilityClass.Get(),
            ApplyLevel,
            INDEX_NONE,
            AbilityEntry.AbilityConfig.Get()
        );

        // 부여한 어빌리티 삭제를 위해 핸들을 저장
        OutWeaponAbilitySpecHandles.AddUnique(GiveAbility(AbilitySpec));
    }
}
