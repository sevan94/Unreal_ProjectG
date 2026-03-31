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

/**
 * 
 */
UCLASS()
class UNREAL_PROJECTG_API USkillAbilityTask : public UAbilityTask
{
	GENERATED_BODY()
	
protected:
    // 액터 큐를 이펙트에 추가하는 유틸리티 함수
    FGameplayEffectSpecHandle AddActorCueIntoSpecHandle(TArray<FGameplayEffectSpecHandle>& SpecHandles, EEffectType InEffectType, const FGameplayTag& InActorCueTag);

    // 스태틱 큐 실행 유틸리티 함수
    void ExecuteSstaticCue(AActor* TargetActor, const FGameplayTag& InStaticCueTag, EEffectType InEffectType, const FGameplayEffectContextHandle& EffectContext) const;

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
    void EmitRuntimeEvent(const FGameplayTag& EventTag, const FGameplayAbilityTargetDataHandle& TargetData = FGameplayAbilityTargetDataHandle());
};
