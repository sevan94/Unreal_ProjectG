// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Types/AbilityConfig.h"
#include "DataAsset_HeroSkillData.generated.h"

class UAbilitySystemComponent;

// 스킬 액션 타입
UENUM(BlueprintType)
enum class ESkillActionType : uint8
{
    SpawnActor      UMETA(DisplayName = "액터 소환"),
    MeleeTrace      UMETA(DisplayName = "근접 공격(트레이스)")
};

//==========================================================================================================
// HeroAbility 관련 Enum
//==========================================================================================================

UENUM(BlueprintType)
enum class EHeroSkillRuntimeTrigger : uint8
{
    None                    UMETA(DisplayName = "없음"),
    OnSkillStart            UMETA(DisplayName = "스킬 시작 시"),
    OnHit                   UMETA(DisplayName = "히트 시"),
    OnKill                  UMETA(DisplayName = "킬 시"),
    OnSkillEnd              UMETA(DisplayName = "스킬 종료 시"),
};

UENUM(BlueprintType)
enum class EHeroSkillRuntimeOpType : uint8
{
    AddAction               UMETA(DisplayName = "액션 추가(뒤)"),
    InsertActionBefore      UMETA(DisplayName = "액션 삽입(앞)"),
    InsertActionAfter       UMETA(DisplayName = "액션 삽입(뒤)"),
    OverrideAction          UMETA(DisplayName = "액션 완전 덮어쓰기"),
    OverrideActionValue     UMETA(DisplayName = "액션 값 덮어쓰기"),
    OverrideEffectValue     UMETA(DisplayName = "이펙트 값 덮어쓰기"),
    ApplyEffects            UMETA(DisplayName = "이펙트 적용"),
};

UENUM(BlueprintType)
enum class EHeroSkillActionScalarField : uint8
{
    SpawnRadius             UMETA(DisplayName = "소환 반경"),
    SpawnLifeSpan           UMETA(DisplayName = "소환 지속 시간"),
    SpawnHtisPerLifeSpan    UMETA(DisplayName = "소환 지속 시간 동안 히트 횟수"),
    TraceRadius             UMETA(DisplayName = "트레이스 반경"),
    TraceOffsetRange        UMETA(DisplayName = "트레이스 사거리"),
    TraceMaxHit             UMETA(DisplayName = "최대 히트 수"),
};

UENUM(BlueprintType)
enum class EHeroSkillEffectScalarField : uint8
{
    EffectMultiplier        UMETA(DisplayName = "이펙트 배수"),
    EffectBaseAmount        UMETA(DisplayName = "이펙트 기본 수치"),
    EffectDuration          UMETA(DisplayName = "이펙트 지속 시간"),
};

// 기본 액션 하나에 대한 단위
USTRUCT(BlueprintType)
struct FSkillActionRow
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "기본 액션", meta = (DisplayName = "액션 ID"))
    FName ActionID = NAME_None;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "기본 액션", meta = (DisplayName = "액션 타입"))
    ESkillActionType ActionType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "기본 액션", meta = (DisplayName = "소환 액션 설정", EditCondition = "ActionType == ESkillActionType::SpawnActor", EditConditionHides))
    FHeroSpawnableConfig SpawnableConfig;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "기본 액션", meta = (DisplayName = "근접 액션 설정", EditCondition = "ActionType == ESkillActionType::MeleeTrace", EditConditionHides))
    FHeroMeleeTraceConfig MeleeTraceConfig;
};

// 런타임 오퍼레이터
USTRUCT(BlueprintType)
struct FHeroSkillRuntimeOp
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "세트 보너스|오퍼레이터", meta = (DisplayName = "오퍼레이터 타입"))
    EHeroSkillRuntimeOpType OpType = EHeroSkillRuntimeOpType::AddAction;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "세트 보너스|오퍼레이터", 
        meta = (DisplayName = "대상 액션 ID", EditCondition = "OpType == EHeroSkillRuntimeOpType::InsertActionBefore || OpType == EHeroSkillRuntimeOpType::InsertActionAfter || OpType == EHeroSkillRuntimeOpType::OverrideActionValue", EditConditionHides)) // 이 Op가 대상 액션이 필요한 타입일 때만 보이는 설정
    FName TargetActionID = NAME_None;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "세트 보너스|오퍼레이터", meta = (DisplayName = "삽입/추가 액션", EditCondition = "OpType == EHeroSkillRuntimeOpType::AddAction || OpType == EHeroSkillRuntimeOpType::InsertActionBefore || OpType == EHeroSkillRuntimeOpType::InsertActionAfter", EditConditionHides)) // OverrideActionValue 타입이 아닐 때만 보이는 설정
    FSkillActionRow ActionToInject;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "런타임 오퍼레이션", meta = (DisplayName = "교체 액션", EditCondition = "OpType == EHeroSkillRuntimeOpType::OverrideAction", EditConditionHides))
    FSkillActionRow OverrideActionRow;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "세트 보너스|오퍼레이터", meta = (DisplayName = "액션 수치 항목", EditCondition = "OpType == EHeroSkillRuntimeOpType::OverrideActionValue", EditConditionHides)) // OverrideActionValue 타입일 때만 보이는 설정
    EHeroSkillActionScalarField ActionScalarField = EHeroSkillActionScalarField::SpawnRadius; // 덮어쓰기할 값의 종류
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "세트 보너스|오퍼레이터", meta = (DisplayName = "덮어쓰기할 값", EditCondition = "OpType == EHeroSkillRuntimeOpType::OverrideActionValue", EditConditionHides)) // OverrideActionValue 타입일 때만 보이는 설정
    FScalableFloat ActionScalarValue = FScalableFloat(0.f); // 덮어쓰기할 값
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "세트 보너스|오퍼레이터", meta = (DisplayName = "이펙트 인덱스(-1이면 전체)", EditCondition = "OpType == EHeroSkillRuntimeOpType::OverrideEffectValue", EditConditionHides)) // OverrideEffectValue 타입일 때만 보이는 설정
    int32 EffectIndex = -1;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "세트 보너스|오퍼레이터", meta = (DisplayName = "이펙트 수치 항목", EditCondition = "OpType == EHeroSkillRuntimeOpType::OverrideEffectValue", EditConditionHides)) // OverrideEffectValue 타입일 때만 보이는 설정
    EHeroSkillEffectScalarField EffectScalarField = EHeroSkillEffectScalarField::EffectMultiplier; // 덮어쓰기할 값의 종류

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "세트 보너스|오퍼레이터", meta = (DisplayName = "이펙트 수치 값", EditCondition = "OpType == EHeroSkillRuntimeOpType::OverrideEffectValue", EditConditionHides)) // OverrideEffectValue 타입일 때만 보이는 설정
    FScalableFloat EffectScalarValue = FScalableFloat(0.f); // 덮어쓰기할 값

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "세트 보너스|오퍼레이터", meta = (DisplayName = "적용할 이펙트 목록", EditCondition = "OpType == EHeroSkillRuntimeOpType::ApplyEffects", EditConditionHides)) // ApplyEffects 타입일 때만 보이는 설정
    TArray<FEffectConfig> Effects;
};

// 세트 보너스 규칙
USTRUCT(BlueprintType)
struct FHeroSkillSetBonusModifier
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "세트 보너스", meta = (DisplayName = "발동 트리거"))
    EHeroSkillRuntimeTrigger Trigger = EHeroSkillRuntimeTrigger::OnHit;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "세트 보너스", meta = (DisplayName = "요구 세트 태그", Categories = "Equipment.Set"))
    FGameplayTag RequiredSetTag;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "세트 보너스", meta = (DisplayName = "요구 세트 개수", ClampMin = "1", ClampMax = "3"))
    int32 RequiredSetPieceCount = 3;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "세트 보너스", meta = (DisplayName = "동일 세트 최고 단계만 적용"))
    bool bUseHighestTierOnly = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "세트 보너스", meta = (DisplayName = "보정 계수(커브/테이블)"))
    FScalableFloat EffectCoefficientMultiplier = FScalableFloat(1.f);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "세트 보너스", meta = (DisplayName = "오퍼레이션 목록"))
    TArray<FHeroSkillRuntimeOp> Ops;
};

/**
 * 
 */
UCLASS()
class UNREAL_PROJECTG_API UDataAsset_HeroSkillData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "기본", meta = (DisplayName = "기본 액션 시퀀스"))
    TArray<FSkillActionRow> ActionSequence;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "세트 보너스", meta = (DisplayName = "세트 보너스 규칙"))
    TArray<FHeroSkillSetBonusModifier> SetBonusModifiers;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "런타임 제한", meta = (DisplayName = "최대 런타임 액션 수", ClampMin = "1"))
    int32 MaxRuntimeActionCount = 64;

public:
    UFUNCTION(BlueprintCallable, Category = "스킬 데이터")
    void BuildRuntimeActionSequence(const UAbilitySystemComponent* AbilitySystem, int32 InAbilityLevel, TArray<FSkillActionRow>& OutRuntimeActions, EHeroSkillRuntimeTrigger InTrigger = EHeroSkillRuntimeTrigger::OnSkillStart) const;

private:
    static int32 FindUniqueActionIndexById(const TArray<FSkillActionRow>& Actions, const FName& ActionId);
    static TArray<FEffectConfig>* GetMutableEffects(FSkillActionRow& Action);
    static void OverrideActionScalar(FSkillActionRow& Action, EHeroSkillActionScalarField Field, float Value);
    static void OverrideEffectScalar(TArray<FEffectConfig>& Effects, int32 EffectIndex, EHeroSkillEffectScalarField Field, float Value);
    static void MultiplyAllEffectMultipliers(TArray<FSkillActionRow>& Actions, float MultiplierAtLevel);
    static bool IsOpSupportedByTrigger(EHeroSkillRuntimeTrigger Trigger, EHeroSkillRuntimeOpType OpType);
};
