// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "HeroCombatInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UHeroCombatInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class UNREAL_PROJECTG_API IHeroCombatInterface
{
	GENERATED_BODY()

public:
    // 기본 공격 속도 반환 함수
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    float GetBasicAttackSpeed() const;
    // 기본 공격 범위 반환 함수
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    float GetBasicAttackRange() const;
    // 기본 공격 실행 시도 함수
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    bool TryExecuteBasicAttack();
    // 액티브 스킬 실행 시도 함수
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    bool TryExecuteActiveSkill();
};
