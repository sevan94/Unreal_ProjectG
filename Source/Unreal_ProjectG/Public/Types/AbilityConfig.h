// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "StructUtils/InstancedStruct.h"
#include "ScalableFloat.h"
#include "GameplayTagContainer.h"
#include "Types/PGEnumTypes.h"
#include "AbilityConfig.generated.h"

class APGProjectileBase;
class UNiagaraComponent;
class UGameplayEffect;
class UPGGameplayAbility;
class APetCharacter;
class AAOESkillActor;
/**
 * 유닛과 캐릭터의 어빌리티의 변수들을 담는 구조체
 */


 // 어빌리티 클래스와 그에 대한 설정을 담는 구조체
USTRUCT(BlueprintType)
struct FAbilityEntry
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TSubclassOf<UPGGameplayAbility> AbilityClass;

    UPROPERTY(EditAnywhere, meta=(BaseStruct="/Script/Unreal_ProjectG.AbilityConfig"))
    FInstancedStruct AbilityConfig;
};

//// 버프를 적용하는 이펙트 클래스와 그에 대한 설정을 담는 구조체
USTRUCT(BlueprintType)
struct FBuffEffectConfig
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    EBuffDurationType BuffDurationType = EBuffDurationType::Infinite;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TSubclassOf<UGameplayEffect> BuffEffectClass;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FScalableFloat SkillMultiplier;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FScalableFloat BaseBuffAmount;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (EditCondition = "BuffDurationType == EBuffDurationType::Duration")) // 버프 지속시간이 Duration일 때만 보이는 설정
    FScalableFloat BuffDuration;
};

//==========================================================================================================
//==========================================================================================================
//==========================================================================================================
// 모든 어빌리티 설정의 부모 구조체, FInstancedStruct로 구조체도 다형성을 가지게 하기 위해서 만들어짐
USTRUCT(BlueprintType)
struct FAbilityConfig
{
	GENERATED_BODY()

    virtual ~FAbilityConfig() = default;
};

//==========================================================================================================
// 히어로 어빌리티 설정 구조체들
//==========================================================================================================
USTRUCT(BlueprintType)
struct FHeroMeleeAttackAbilityConfig : public FAbilityConfig
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TSubclassOf<UGameplayEffect> DamageEffectClass; // 데미지 계산 클래스

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FScalableFloat SkillMultiplier; // 스킬 계수

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 MaxHitTargets = 1; // 최대 공격 가능한 적의 수

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float WeaponTraceRadius = 50.f; // 무기 트레이스 반경

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<TSoftObjectPtr<UAnimMontage>> MeleeAttackMontages; // 공격 애니메이션 몽타주들

    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Categories = "GameplayCue"))
    FGameplayTag HitImpactCueTag; // 공격이 적중했을 때 재생할 이펙트 태그
};

USTRUCT(BlueprintType)
struct FHeroSpawnProjectileAbilityConfig : public FAbilityConfig
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TSubclassOf<UGameplayEffect> DamageEffectClass; // 데미지 계산 클래스

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FScalableFloat SkillMultiplier; // 스킬 계수

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TSoftClassPtr<APGProjectileBase> SpawnedProjectileClass; // 스폰할 프로젝타일 클래스

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<TSoftObjectPtr<UAnimMontage>> SpawnProjectileMontages; // 공격 애니메이션 몽타주들

    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Categories = "GameplayCue"))
    FGameplayTag SpawnCueTag; // 스폰할때, 재생할 이펙트 태그
};

// AOE 어빌리티 설정 구조체
USTRUCT(BlueprintType)
struct FHeroAOECommonConfig : public FAbilityConfig
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    EAOETargetPolicy TargetPolicy = EAOETargetPolicy::HostileOnly; // AOE 공격의 타겟팅 정책

    UPROPERTY(EditAnywhere, BlueprintReadOnly) // 아군에게 데미지를 입힐일은 없으니 데미지 계산 클래스는 아군 공격이 아닐 때만 보이도록
    TSubclassOf<UGameplayEffect> InstantEffectClass; // 데미지 계산 클래스

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TSubclassOf<UGameplayEffect> BuffDebuffClass; // 버프/디버프 클래스

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TSubclassOf<AAOESkillActor> SpawnedActorClass; // 스폰할 액터 클래스

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TSoftObjectPtr<UAnimMontage> Montage; // 캐스팅 애니메이션 몽타주들

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TObjectPtr<UMaterialInterface> AOEIndicatorDecalMaterial; // AOE 범위를 보여주는 데칼 머티리얼
};

USTRUCT(BlueprintType)
struct FHeroSpawnPetAbilityConfig : public FAbilityConfig
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<TSoftClassPtr<APetCharacter>> SpawnedPetClasses; // 스폰할 펫 클래스
};


//==========================================================================================================
// 유닛 어빌리티 설정 구조체들
//==========================================================================================================
USTRUCT(BlueprintType)
struct FUnitBaseMeleeAttackAbilityConfig : public FAbilityConfig
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TSubclassOf<UGameplayEffect> DamageEffectClass; // 데미지 계산 클래스

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FScalableFloat SkillMultiplier; // 스킬 계수

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 MaxHitTargets = 1; // 최대 공격 가능한 적의 수

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float MeleeAttackDamageRadius = 100.f; // 공격 트레이스 반경

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<TSoftObjectPtr<UAnimMontage>> MeleeAttackMontages; // 공격 애니메이션 몽타주들

    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Categories = "GameplayCue"))
    FGameplayTag HitImpactCueTag; // 공격이 적중했을 때 재생할 이펙트 태그
};

USTRUCT(BlueprintType)
struct FUnitSpawnProjectileAbilityConfig : public FAbilityConfig
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TSubclassOf<UGameplayEffect> DamageEffectClass; // 데미지 계산 클래스

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FScalableFloat SkillMultiplier; // 스킬 계수

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TSoftClassPtr<APGProjectileBase> SpawnedProjectileClass; // 스폰할 프로젝타일 클래스

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<TSoftObjectPtr<UAnimMontage>> SpawnProjectileMontages; // 공격 애니메이션 몽타주들

    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Categories = "GameplayCue"))
    FGameplayTag SpawnCueTag; //스폰할때, 재생할 이펙트 태그
};

USTRUCT(BlueprintType)
struct FUnitSpawnActorAbilityConfig : public FAbilityConfig
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TSubclassOf<UGameplayEffect> DamageEffectClass; 

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FScalableFloat SkillMultiplier;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TSoftClassPtr<AActor> SpawnedActorClass; 

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 SpawnCount;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<TSoftObjectPtr<UAnimMontage>> SpawnActorMontages;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Categories = "GameplayCue"))
    FGameplayTag SpawnCueTag; 
};

USTRUCT(BlueprintType)
struct FSharedBuffAuraAbilityConfig : public FAbilityConfig
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<FBuffEffectConfig> Buffs; // 수치 버프량 계산을 위한 데이터 배열

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float BuffAuraRadius; // 버프 오라의 반경

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TSoftObjectPtr<UMaterialInterface> AuraRadiusDecalMaterial; // 버프 오라의 범위를 보여주는 데칼 머티리얼
};

USTRUCT(BlueprintType)
struct FUnitBuffAuraAbilityConfig : public FAbilityConfig
{
    GENERATED_BODY()

    // 장판 반경 내 아군에게 부여할 힐/버프 이펙트 (GE)
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability|Support")
    TSubclassOf<UGameplayEffect> SupportEffectClass;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TSoftObjectPtr<class UNiagaraSystem> BuffEffect;

    // 시전 애니메이션
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability|Support")
    TObjectPtr<UAnimMontage> SupportMontage;

    // 힐/버프 계수
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability|Support")
    FScalableFloat SupportSkillMultiplier;

    // 지원 유닛 주위에 깔릴 장판의 탐색 반경
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability|Support")
    float SupportRadius = 500.f;

    // 장판이 유닛을 따라다닐지(오라 형태), 시전 위치에 남을지 결정
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability|Support")
    bool bAttachToUnit = false;
};
