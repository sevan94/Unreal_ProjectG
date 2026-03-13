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
struct FNumericBuffEffectConfig
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Categories = "Attribute_SetByCaller"))
    FGameplayTag BuffTag; // 버프/디버프의 태그, 예: "Buff.AttackPower", "Debuff.Defense"

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FScalableFloat SkillMultiplier;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FScalableFloat BaseBuffAmount;
};

//// 버프/디버프 설정 구조체, 수치형 버프/디버프와 상태이상 버프/디버프를 모두 담을 수 있도록 함
//USTRUCT(BlueprintType)
//struct FBuffDebuffConfig
//{
//    GENERATED_BODY()
//
//    //수치형 버프/디버프 (공격력, 체력 등)
//    UPROPERTY(EditAnywhere, BlueprintReadOnly)
//    TArray<FNumericBuffEffectConfig> NumericBuffs;
//
//    //상태이상 버프/디버프
//    UPROPERTY(EditAnywhere, BlueprintReadOnly)
//    TArray<TSubclassOf<UGameplayEffect>> StatusEffectClasses;
//
//    //지속시간
//    UPROPERTY(EditAnywhere, BlueprintReadOnly)
//    FScalableFloat Duration;
//};

//// 데미지 설정 구조체, 데미지 계산에 필요한 정보들을 담음
//USTRUCT(BlueprintType)
//struct FDamageConfig
//{
//    GENERATED_BODY()
//
//    UPROPERTY(EditAnywhere, BlueprintReadOnly)
//    TSubclassOf<UGameplayEffect> DamageEffectClass; // 데미지 계산 클래스
//
//    UPROPERTY(EditAnywhere, BlueprintReadOnly)
//    FScalableFloat SkillMultiplier; // 스킬 계수
//};

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

    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (EditCondition = "TargetPolicy != EAOETargetPolicy::FriendlyOnly")) // 아군에게 데미지를 입힐일은 없으니 데미지 계산 클래스는 아군 공격이 아닐 때만 보이도록
    TSubclassOf<UGameplayEffect> DamageEffectClass; // 데미지 계산 클래스

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TSubclassOf<UGameplayEffect> BuffDebuffClass; // 버프/디버프 클래스

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<TSubclassOf<UGameplayEffect>> StatusEffectClasses; // 상태이상 버프/디버프 클래스

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
    TSoftClassPtr<APetCharacter> SpawnedPetClass; // 스폰할 펫 클래스
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
    TArray<FNumericBuffEffectConfig> NumericBuffs; // 수치 버프량 계산을 위한 데이터 배열

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<TSubclassOf<UGameplayEffect>> StatusEffectClasses; // 상태형 버프

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float BuffAuraRadius; // 버프 오라의 반경

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TSoftObjectPtr<UMaterialInterface> AuraRadiusDecalMaterial; // 버프 오라의 범위를 보여주는 데칼 머티리얼
};

//UCLASS(EditInlineNew, BlueprintType)
//class UNREAL_PROJECTG_API UUnitSpawnNiagaraConfig : public UAbilityConfig
//{
//    GENERATED_BODY()
//
//public:
//    // 프로젝타일 스폰 어빌리티 데이터
//    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
//    TSubclassOf<UNiagaraComponent> SpawnedActorClass;
//};