// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameplayTagContainer.h"
#include "DataAssets/Ability/AbilityConfig.h"
#include "PGStructTypes.generated.h"

class UPGHeroGameplayAbility;
class UPGHeroLinkedAnimLayer;
class UPGGameplayAbility;
/**
 * 
 */

USTRUCT(BlueprintType)
struct FAbilityEntry
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TSubclassOf<UPGGameplayAbility> AbilityClass;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Instanced, meta = (AllowAbstract = false, ShowOnlyInnerProperties))
    TObjectPtr<UAbilityConfig> AbilityConfig;
};

// 무기 데이터 구조체
USTRUCT(BlueprintType)
struct FPGHeroWeaponData
{
    GENERATED_BODY()

    // 무기 스태틱 메시
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TSoftObjectPtr<UStaticMesh> SoftWeaponMesh;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FName WeaponSocketName;

    // 무기 애니메이션 레이어
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TSubclassOf<UPGHeroLinkedAnimLayer> WeaponAnimLayer;    
    
    // 무기 기본 경격 어빌리티
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (TitleProperty = "InputTag"))
    TSubclassOf<UPGHeroGameplayAbility> BaseAttackAbility;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FAbilityEntry WeaponBasicAttackAbilityEntry;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<FAbilityEntry> WeaponSkillAbilityEntries;

    // 무기 스킬 어빌리티들
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TArray<TSubclassOf<UPGHeroGameplayAbility>> WeaponSkillAbilities;

    // 무기 아이콘 텍스처
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TSoftObjectPtr<UTexture2D> SoftWeaponIconTexture;
};

//// 방어구 데이터 구조체
//USTRUCT(BlueprintType)
//struct FPGHeroArmorData
//{
//    GENERATED_BODY()
//
//    FScalableFloat
//};