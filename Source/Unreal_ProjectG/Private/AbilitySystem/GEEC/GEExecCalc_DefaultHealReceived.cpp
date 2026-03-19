// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/GEEC/GEExecCalc_DefaultHealReceived.h"
#include "AbilitySystem/PGCharacterAttributeSet.h"
#include "PGGameplayTags.h"

struct FPGHealReceivedCapture
{
    // 캡처할 어트리뷰트 정의를 추가
    DECLARE_ATTRIBUTE_CAPTUREDEF(HealReceived)
    DECLARE_ATTRIBUTE_CAPTUREDEF(AttackPower)

    FPGHealReceivedCapture()
    {
        // 어트리뷰트 캡처 설정
        DEFINE_ATTRIBUTE_CAPTUREDEF(UPGCharacterAttributeSet, HealReceived, Target, false);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UPGCharacterAttributeSet, AttackPower, Source, false);
    }
};

// 싱글톤 패턴으로 FPGHealReceivedCapture 인스턴스 반환
static const FPGHealReceivedCapture& GetPGHealReceivedCapture()
{
    static FPGHealReceivedCapture PGHealReceivedCapture;
    return PGHealReceivedCapture;
}

UGEExecCalc_DefaultHealReceived::UGEExecCalc_DefaultHealReceived()
{
    // 캡처할 어트리뷰트 정의를 RelevantAttributesToCapture 배열에 추가
    RelevantAttributesToCapture.Add(GetPGHealReceivedCapture().HealReceivedDef);
    RelevantAttributesToCapture.Add(GetPGHealReceivedCapture().AttackPowerDef);
}

void UGEExecCalc_DefaultHealReceived::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
    const FGameplayEffectSpec& EffectSpec = ExecutionParams.GetOwningSpec();

    // 소스와 타깃의 태그들을 가져와서 구조체에 저장
    FAggregatorEvaluateParameters EvaluationParameters;
    EvaluationParameters.SourceTags = EffectSpec.CapturedSourceTags.GetAggregatedTags();
    EvaluationParameters.TargetTags = EffectSpec.CapturedTargetTags.GetAggregatedTags();
    
    // 캡처된 어트리뷰트에서 원하는 값을 가져와서 할당
    float SourceAttackPower = 0.0f;
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetPGHealReceivedCapture().AttackPowerDef, EvaluationParameters, SourceAttackPower);

    float BaseHealAmount = SourceAttackPower;
    float SkillHealMultiplier = 1.0f;

    for(const TPair<FGameplayTag, float>& TagMagnitude : EffectSpec.SetByCallerTagMagnitudes)
    {
        if (TagMagnitude.Key.MatchesTagExact(PGGameplayTags::Shared_SetByCaller_SkillMultiplier))
        {
            SkillHealMultiplier = TagMagnitude.Value;
        }
    }

    //============================================================
    // 최종 힐량 계산
    //============================================================
    if (SkillHealMultiplier != 1.0f)
    {
        BaseHealAmount *= SkillHealMultiplier;
    }

    const float FinalHealAmount = BaseHealAmount;

    UE_LOG(LogTemp, Log, TEXT("Final Heal Amount: %f"), FinalHealAmount);

    if (FinalHealAmount > 0.0f)
    {
        OutExecutionOutput.AddOutputModifier(
            FGameplayModifierEvaluatedData(
                GetPGHealReceivedCapture().HealReceivedDef.AttributeToCapture,
                EGameplayModOp::Additive,
                FinalHealAmount
            )
        );
    }
}