// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "Types/PGEnumTypes.h"
#include "SkillAbilityTask.generated.h"

// 모든 스킬 Task 공통 델리게이트 
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSkillTaskDelegate, FGameplayAbilityTargetDataHandle, TargetData);
// 스킬 런타임 이벤트 델리게이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSkillRuntimeEventDelegate, FGameplayTag, EventTag, FGameplayAbilityTargetDataHandle, TargetData);

struct FEffectConfig;

/**
 * 
 */
UCLASS()
class UNREAL_PROJECTG_API USkillAbilityTask : public UAbilityTask
{
	GENERATED_BODY()
	
protected:
    // Actor Cue: GE Spec에 PGEffectContext(CueEffectType) 주입
    FGameplayEffectContextHandle  AddActorCueIntoSpecHandle(FGameplayEffectSpecHandle& InOutSpecHandle, const FEffectConfig& InEffectConfig) const;

    // Static Cue: GE와 분리 실행, CueParams.EffectContext에 CueEffectType 담아 전달
    void ExecuteStaticCue(AActor* TargetActor, const FEffectConfig& InEffectConfig, const FGameplayEffectContextHandle& EffectContext) const;

public:
    // Task 완료(정상 종료)
    UPROPERTY(BlueprintAssignable)
    FSkillTaskDelegate OnCompleted;

    // Task 실패(예외 종료)
    UPROPERTY(BlueprintAssignable)
    FSkillTaskDelegate OnCancelled;

    // 스킬 런타임 이벤트(히트, 액터 틱 등) 수신용 델리게이트
    UPROPERTY(BlueprintAssignable)
    FSkillRuntimeEventDelegate OnRuntimeEvent;

protected:
    // 스킬 런타임 이벤트를 수신하는 헬퍼 함수
    void EmitRuntimeEvent(const FGameplayTag& EventTag, const FGameplayAbilityTargetDataHandle& TargetData = FGameplayAbilityTargetDataHandle());
};
