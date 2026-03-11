// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/GEEC/GEExecCalc_DefaultDamageTaken.h"
#include "AbilitySystem/PGCharacterAttributeSet.h"
#include "PGGameplayTags.h"

struct FPGDamageCapture
{
    // 캡처할 어트리 뷰트 정의 추가
    DECLARE_ATTRIBUTE_CAPTUREDEF(DamageTaken)
    DECLARE_ATTRIBUTE_CAPTUREDEF(AttackPower)

    FPGDamageCapture()
    {
        // 어트리뷰트 캡처 설정
        DEFINE_ATTRIBUTE_CAPTUREDEF(UPGCharacterAttributeSet, AttackPower, Source, false);
    }
};

// 싱글톤 패턴으로 FPGDamageCapture 인스턴스 반환
static const FPGDamageCapture& GetPGDamageCapture()
{
    static FPGDamageCapture PGDamageCapture;
    return PGDamageCapture;
}

UGEExecCalc_DefaultDamageTaken::UGEExecCalc_DefaultDamageTaken()
{
    // 캡처할 어트리뷰트 정의를 RelevantAttributesToCapture 배열에 추가
    RelevantAttributesToCapture.Add(GetPGDamageCapture().AttackPowerDef);
}

void UGEExecCalc_DefaultDamageTaken::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
    const FGameplayEffectSpec& EffectSpec = ExecutionParams.GetOwningSpec();

    // 소스와 타깃의 태그들을 가져와서 구조체에 저장
    FAggregatorEvaluateParameters EvaluationParameters;
    EvaluationParameters.SourceTags = EffectSpec.CapturedSourceTags.GetAggregatedTags();
    EvaluationParameters.TargetTags = EffectSpec.CapturedTargetTags.GetAggregatedTags();

    // ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(...)
    // 캡처된 어트리뷰트 중에서 우리가 원하는 값을 계산해서 가져와
    // GetPGDamageCapture().AttackPowerDef: 어떤 어트리뷰트를 가져올지 지정하는 정의
    // EvaluationParameters :  Source / Target 태그 정보
    // SourceAttackPower(Out 파라미터) : 함수가 성공적으로 계산을 마치면, 그 결과값이 이 변수에 할당  
    float SourceAttackPower = 0.0f;
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetPGDamageCapture().AttackPowerDef, EvaluationParameters, SourceAttackPower);

    float BaseDamage = SourceAttackPower;
    float SkillDamageMultiplier = 1.0f;

    // SetByCaller로 넘겨준 Map(태그, 값)에서 태그를 찾아서 값을 가져옴 
    for (const TPair<FGameplayTag, float>& TagMagnitude : EffectSpec.SetByCallerTagMagnitudes)
    {
        if (TagMagnitude.Key.MatchesTagExact(PGGameplayTags::Shared_SetByCaller_SkillMultiplier))
        {
            SkillDamageMultiplier = TagMagnitude.Value;
        }
    }

    // 방어력 등 추가 스탯이 생기면 여기에서 계산 로직 추가

    //================================================
    // 최종 데미지 계산
    //================================================
    if (SkillDamageMultiplier != 1)
    {
        BaseDamage *= SkillDamageMultiplier;
    }

    const float FinalDamage = BaseDamage;

    UE_LOG(LogTemp, Warning, TEXT("Final Damage Taken : %f"), FinalDamage);

    if (FinalDamage > 0.f)
    {
        OutExecutionOutput.AddOutputModifier(
            FGameplayModifierEvaluatedData(
                UPGCharacterAttributeSet::GetDamageTakenAttribute(),
                EGameplayModOp::AddBase,
                FinalDamage
            )
        );
    }
}
