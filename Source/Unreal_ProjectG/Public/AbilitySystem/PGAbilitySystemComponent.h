// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "PGAbilitySystemComponent.generated.h"

class UPGHeroGameplayAbility;
class UDataAsset_SkillData;
/**
 * 
 */
UCLASS()
class UNREAL_PROJECTG_API UPGAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
	

public:
    // 어빌리티 데이터 에셋로부터 단일 어빌리티 부여(핸들 반환)
    UFUNCTION(BlueprintCallable, Category = "Abilities", meta = (ApplyLevel = "1"))
    void GrantHeroAbilityByAbilityData(const TSoftObjectPtr<UDataAsset_SkillData>& InAbilityData, int32 ApplyLevel);

    // 어빌리티 데이터 에셋 배열로부터 어빌리티 부여(핸들 반환)
    UFUNCTION(BlueprintCallable, Category = "Abilities", meta = (ApplyLevel = "1"))
    void GrantHeroAbilitiesByAbilityData(const TArray<TSoftObjectPtr<UDataAsset_SkillData>>& InAbilityDataArray, int32 ApplyLevel, TArray<FGameplayAbilitySpecHandle>& OutWeaponAbilitySpecHandles);

    // 태그로 어빌리티 활성화 시도
    UFUNCTION(BlueprintCallable, Category = "Abilities")
    bool TryActivateAbilityByTag(FGameplayTag AbilityTagToActivate);
};
