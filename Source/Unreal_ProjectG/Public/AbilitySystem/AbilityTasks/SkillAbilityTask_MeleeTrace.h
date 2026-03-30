// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/AbilityTasks/SkillAbilityTask.h"
#include "DataAssets/Ability/DataAsset_HeroSkillData.h"
#include "SkillAbilityTask_MeleeTrace.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_PROJECTG_API USkillAbilityTask_MeleeTrace : public USkillAbilityTask
{
	GENERATED_BODY()
	
public:
    // Task 생성 함수
    static USkillAbilityTask_MeleeTrace* Create(UGameplayAbility* OwningAbility, const FSkillActionRow& ActionRow);

    virtual void Activate() override;
    virtual void OnDestroy(bool bInOwnerFinished) override;

private:
    // 몽타주
    UFUNCTION()
    void OnMontageCancelled();

    // 트레이스 활성화/비활성화
    UFUNCTION()
    void OnTraceStartEventReceived(FGameplayEventData Payload);

    UFUNCTION()
    void OnTraceEndEventReceived(FGameplayEventData Payload);

    // 트레이스 활성화 동안 반복 호출
    void ExecuteTrace();

    bool IsValidTarget(AActor* Target) const;
private:
    FSkillActionRow CachedActionRow; // 액션 데이터 캐싱

    // 트레이스 활성화 여부
    bool bIsTraceActive = false;

    // 트레이스 타이머 핸들
    FTimerHandle TraceTimerHandle;

    // 히트 대상 중복 방지 캐싱
    TSet<TObjectPtr<AActor>> HitActors;

    // 이전 프레임 소켓 위치 캐싱(스윕)
    FVector PreviousTraceStart = FVector::ZeroVector;
    FVector PreviousTraceEnd = FVector::ZeroVector;

    bool bHitEventFlag = false; // 첫 번째 히트 발생을 체크하기 위한 플래그
};
