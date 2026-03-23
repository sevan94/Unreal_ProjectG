// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/PGAbilitySystemComponent.h"
#include "AbilitySystem/Abilities/PGHeroGameplayAbility.h"
#include "DataAssets/Ability/DataAsset_SkillData.h"

bool UPGAbilitySystemComponent::TryActivateAbilityByTag(FGameplayTag AbilityTagToActivate)
{
    check(AbilityTagToActivate.IsValid());

    //// 어빌리티 태그와 일치하는 모든 어빌리티 스펙 찾음
    //const TArray<FGameplayAbilitySpec>& AllSpecs = GetActivatableAbilities();
    //for (const FGameplayAbilitySpec& Spec : AllSpecs)
    //{
    //    // 2. 기본 어빌리티 태그 확인 (예: Skill.Active.FireBall)
    //    if (Spec.Ability)
    //    {
    //        FGameplayTagContainer SpecTags = Spec.GetDynamicSpecSourceTags();
    //        UE_LOG(LogTemp, Log, TEXT("Checking Spec: %s | Tags: %s"),
    //            *Spec.Ability->GetName(),
    //            *SpecTags.ToString());
    //    }
    //}

    // ASC 내부 또는 유닛 클래스 내부 로직
    FGameplayAbilitySpec* FoundSpec = nullptr;

    for (FGameplayAbilitySpec& Spec : GetActivatableAbilities())
    {
        // 1. 해당 스펙이 유효한지 확인
        if (!Spec.Ability) continue;

        // 2. Dynamic Tag Container에서 해당 태그가 있는지 확인
        if (Spec.GetDynamicSpecSourceTags().HasTag(AbilityTagToActivate))
        {
            FoundSpec = &Spec;
            break;
        }
    }

    if (FoundSpec)
    {
        // 찾은 스펙으로 직접 실행 시도
        TryActivateAbility(FoundSpec->Handle);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("태그 [%s]를 가진 어빌리티를 찾을 수 없습니다."), *AbilityTagToActivate.ToString());
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
