// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "StructUtils/InstancedStruct.h"
#include "ScalableFloat.h"
#include "GameplayTagContainer.h"
#include "AbilityConfig.generated.h"

class APGProjectileBase;
class UNiagaraComponent;
class UGameplayEffect;
class UPGGameplayAbility;
class APetCharacter;
/**
 * 유닛과 캐릭터의 어빌리티의 변수들을 담는 구조체
 */


USTRUCT(BlueprintType)
struct FAbilityEntry
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TSubclassOf<UPGGameplayAbility> AbilityClass;

    UPROPERTY(EditAnywhere, meta=(BaseStruct="/Script/Unreal_ProjectG.AbilityConfig"))
    FInstancedStruct AbilityConfig;
};

USTRUCT(BlueprintType)
struct FNumericBuffEffectConfig
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TSoftClassPtr<UGameplayEffect> EffectClass;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FScalableFloat SkillMultiplier;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FScalableFloat BaseBuffAmount;
};

USTRUCT(BlueprintType)
struct FAbilityConfig
{
	GENERATED_BODY()

    virtual ~FAbilityConfig() = default;
};

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
    TSoftClassPtr<UGameplayEffect> DamageEffectClass; // 데미지 계산 클래스

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FScalableFloat SkillMultiplier; // 스킬 계수

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TSoftClassPtr<APGProjectileBase> SpawnedProjectileClass; // 스폰할 프로젝타일 클래스

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<TSoftObjectPtr<UAnimMontage>> SpawnProjectileMontages; // 공격 애니메이션 몽타주들

    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Categories = "GameplayCue"))
    FGameplayTag SpawnCueTag; // 스폰할때, 재생할 이펙트 태그
};

USTRUCT(BlueprintType)
struct FHeroCastingAOEAbilityConfig : public FAbilityConfig
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TSoftClassPtr<UGameplayEffect> DamageEffectClass; // 데미지 계산 클래스

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FScalableFloat SkillMultiplier; // 스킬 계수

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 MaxHitTargets = 3; // 최대 공격 가능한 적의 수

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float AOEAttackRadius = 300.f; // AOE 공격 반경

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TSoftObjectPtr<UAnimMontage> CastingMontage; // 캐스팅 애니메이션 몽타주들

    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Categories = "GameplayCue"))
    FGameplayTag ImpactCueTag; // 공격이 적중했을 때 재생할 이펙트 태그
};

USTRUCT(BlueprintType)
struct FHeroSpawnPetAbilityConfig : public FAbilityConfig
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TSoftClassPtr<APetCharacter> SpawnedPetClass; // 스폰할 펫 클래스
};


USTRUCT(BlueprintType)
struct FUnitBaseMeleeAttackAbilityConfig : public FAbilityConfig
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TSoftClassPtr<UGameplayEffect> DamageEffectClass; // 데미지 계산 클래스

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
    TSoftClassPtr<UGameplayEffect> DamageEffectClass; // 데미지 계산 클래스

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
    TSoftClassPtr<UGameplayEffect> DamageEffectClass; 

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
    TArray<FNumericBuffEffectConfig> NumericBuffs; // 수치 버프량 계산을 위한 데이터 배열

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<TSoftClassPtr<UGameplayEffect>> StatusEffectClasses; // 상태형 버프

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float BuffAuraRadius; // 버프 오라의 반경

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TSoftObjectPtr<UMaterialInterface> AuraRadiusDecalMaterial; // 버프 오라의 범위를 보여주는 데칼 머티리얼

    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Categories = "GameplayCue"))
    FGameplayTag SpawnCueTag; //스폰할때, 재생할 이펙트 태그
};

USTRUCT(BlueprintType)
struct FUnitBuffAuraAbilityConfig : public FAbilityConfig
{
    GENERATED_BODY()

    // 장판 반경 내 아군에게 부여할 힐/버프 이펙트 (GE)
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability|Support")
    TSubclassOf<UGameplayEffect> SupportEffectClass;

    // 시전 애니메이션
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability|Support")
    TObjectPtr<UAnimMontage> SupportMontage;

    // 힐/버프 계수
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability|Support")
    FScalableFloat SupportSkillMultiplier;

    // 지원 유닛 주위에 깔릴 장판의 탐색 반경
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability|Support")
    float SupportRadius = 500.f;

    // 시각적으로 바닥에 깔아둘 장판 액터 클래스 (데칼이나 파티클을 가진 액터)
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability|Support")
    TSubclassOf<AActor> JangpanActorClass;

    // 장판이 유닛을 따라다닐지(오라 형태), 시전 위치에 남을지 결정
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability|Support")
    bool bAttachToUnit = false;
};
