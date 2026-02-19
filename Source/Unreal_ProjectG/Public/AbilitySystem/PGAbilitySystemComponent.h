// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "Types/PGStructTypes.h"
#include "PGAbilitySystemComponent.generated.h"

class UPGHeroGameplayAbility;
/**
 * 
 */
UCLASS()
class UNREAL_PROJECTG_API UPGAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
	

public:
    // 무기 기본 공격 어빌리티 부여
    UFUNCTION(BlueprintCallable, Category = "Abilities", meta = (ApplyLevel = "1"))
    void GrantHeroWeaponBasicAttackAbility(TSubclassOf<UPGHeroGameplayAbility> InBasicAttackAbility, int32 ApplyLevel, FGameplayAbilitySpecHandle& OutBasicAttackAbilitySpecHandle);

    // 무기 스킬 어빌리티 부여
    UFUNCTION(BlueprintCallable, Category = "Abilities", meta = (ApplyLevel = "1"))
    void GrantHeroWeaponSkillAbilities(const TArray<TSubclassOf<UPGHeroGameplayAbility>>& InWeaponSkillAbilities, int32 ApplyLevel, TArray<FGameplayAbilitySpecHandle>& OutWeaponAbilitySpecHandles);
};
