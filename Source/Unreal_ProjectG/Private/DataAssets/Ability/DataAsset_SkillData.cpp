// Fill out your copyright notice in the Description page of Project Settings.


#include "DataAssets/Ability/DataAsset_SkillData.h"
#include "AbilitySystem/PGAbilitySystemComponent.h"

const FAbilityEntry& UDataAsset_SkillData::GetGivenAbilityEntryForASC(const UAbilitySystemComponent* InASC) const
{
    // ASC의 유효성과 업그레이드 태그가 유효한지 확인
    if (InASC && UpgradeRequirementTag.IsValid())
    {
        // 캐릭터가 세트 아이템태그를 가지고 있는지 검사
        if (InASC->HasMatchingGameplayTag(UpgradeRequirementTag))
        {
            // 세트 효과 활성화
            return UpgradeAbilityEntry;
        }
    }
    // 업그레이드 조건이 충족되지 않은 경우 기본 어빌리티 엔트리를 반환
    return AbilityEntry;
}
