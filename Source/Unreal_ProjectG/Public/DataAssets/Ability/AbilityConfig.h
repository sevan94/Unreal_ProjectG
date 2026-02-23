// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ScalableFloat.h"
#include "GameplayTagContainer.h"
#include "AbilityConfig.generated.h"

class APGProjectileBase;

/**
 * 
 */
UCLASS(Abstract, EditInlineNew, BlueprintType)
class UNREAL_PROJECTG_API UAbilityConfig : public UObject
{
	GENERATED_BODY()

public:
    // 어빌리티 공통 데이터
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TObjectPtr<UAnimMontage> AbilityMontage;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FScalableFloat DamageMultiplier = 1.f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float Cooldown = 0.f;
};

UCLASS(EditInlineNew, BlueprintType)
class UNREAL_PROJECTG_API UMeleeAttackAbilityConfig : public UAbilityConfig
{
    GENERATED_BODY()
public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FGameplayTag HitImpactTag;
};

UCLASS(EditInlineNew, BlueprintType)
class UNREAL_PROJECTG_API USpawnProjectileAbilityConfig : public UAbilityConfig
{
    GENERATED_BODY()

public:
    // 프로젝타일 스폰 어빌리티 데이터
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TSubclassOf<APGProjectileBase> SpawnedProjectileClass;
};

UCLASS(EditInlineNew, BlueprintType)
class UNREAL_PROJECTG_API UUnitMeleeAttackAbilityConfig : public UAbilityConfig
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 MaxHitTargets = 1;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FGameplayTag HitImpactTag;
};