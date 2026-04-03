// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameplayEffectTypes.h"
#include "Types/PGEnumTypes.h"
#include "ScalableFloat.h"
#include "PGGasTypes.generated.h"

/**
 * Gas 헤더가 필요한 타입들을 정의하는 헤더입니다.
 */

UENUM(BlueprintType)
enum class EPGEffectModOp : uint8
{
    Additive UMETA(DisplayName = "Additive"),
    MultiplyAdditive UMETA(DisplayName = "Multiply (Additive)"),
};

USTRUCT(BlueprintType)
struct FPGAttributeModifierEntry
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FGameplayAttribute Attribute;                         // 수정할 속성

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    EPGEffectModOp ModifierOp = EPGEffectModOp::Additive; // 계산 방식(더하기, 곱하기 등)

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FScalableFloat ModifierValue;                         // 계산에 사용할 값
};