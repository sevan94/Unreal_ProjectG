// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/PawnExtensionComponentBase.h"
#include "GamePlayAbilitySpecHandle.h"
#include "PawnCombatComponent.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, meta = (BlueprintSpawnableComponent))
class UNREAL_PROJECTG_API UPawnCombatComponent : public UPawnExtensionComponentBase
{
	GENERATED_BODY()
	
public:
    UPawnCombatComponent();

    UFUNCTION(BlueprintCallable, Category = "Combat")
    const FGameplayAbilitySpecHandle& GetBaseAttackAbilitySpecHandle() const { return GrantedBaseAttackAbilitySpecHandle; }
    
    UFUNCTION(BlueprintCallable, Category = "Combat")
    TArray<FGameplayAbilitySpecHandle> GetSkillAbilitySpecHandles() const { return GrantedSkillAbilitySpecHandles; }

public:
    // 나중에 태그나 다른 타입도 고려
    UPROPERTY(BlueprintReadWrite, Category = "Combat|Property")
    bool bWeaponEquipped = false;

protected:
    // 스킬 어빌리티를 위한 어빌리티 스펙 핸들들
    UPROPERTY(VisibleAnywhere, Category = "Combat|Data")
    TArray<FGameplayAbilitySpecHandle> GrantedSkillAbilitySpecHandles;

    // 기본 공격 어빌리티를 위한 어빌리티 스펙 핸들
    UPROPERTY(VisibleAnywhere, Category = "Combat|Data")
    FGameplayAbilitySpecHandle GrantedBaseAttackAbilitySpecHandle;
};
