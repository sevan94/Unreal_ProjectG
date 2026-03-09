// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/PGAbilitySystemComponent.h"
#include "AbilitySystem/Abilities/PGHeroGameplayAbility.h"
#include "DataAssets/Ability/DataAsset_SkillData.h"

bool UPGAbilitySystemComponent::TryActivateAbilityByTag(FGameplayTag AbilityTagToActivate)
{
    check(AbilityTagToActivate.IsValid());

    // 어빌리티 태그와 일치하는 모든 어빌리티 스펙 찾음
    TArray<FGameplayAbilitySpec*> FoundAbilitySpecs;
    GetActivatableGameplayAbilitySpecsByAllMatchingTags(AbilityTagToActivate.GetSingleTagContainer(), FoundAbilitySpecs);

    if (!FoundAbilitySpecs.IsEmpty())
    {
        //찾은 어빌리티 스펙들 중에서 랜덤으로 하나 활성화(보통 한개만 넣기 때문에 특정 어빌리티 활성화)
        //그러나 만약 기본 공격등을 여러개 넣는다면 랜덤으로 하나 활성화
        const int32 RandomAbilityIndex = FMath::RandRange(0, FoundAbilitySpecs.Num() - 1);
        FGameplayAbilitySpec* SpecToActivate = FoundAbilitySpecs[RandomAbilityIndex];

        check(SpecToActivate);

        // 활성화 중이 아닌 어빌리티라면 활성화 시도
        if (!SpecToActivate->IsActive())
        {
            return TryActivateAbility(SpecToActivate->Handle);
        }
    }

    return false;
}
