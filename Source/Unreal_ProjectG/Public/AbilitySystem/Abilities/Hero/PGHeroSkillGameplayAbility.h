// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/PGHeroGameplayAbility.h"
#include "DataAssets/Ability/DataAsset_HeroSkillData.h"
#include "PGHeroSkillGameplayAbility.generated.h"

/**
 * 히어로 스킬 어빌리티
 * HeroSkillData의 ActionSequence에 따라 스킬 액션들을 순차적으로 실행하는 어빌리티
 */
UCLASS()
class UNREAL_PROJECTG_API UPGHeroSkillGameplayAbility : public UPGHeroGameplayAbility
{
	GENERATED_BODY()

public:
    UPGHeroSkillGameplayAbility();


protected:
    virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
    virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

private:
    void ExecuteNextAction();

    // 런타임 트리거 처리 함수들
    void ExecuteRuntimeTrigger(EHeroSkillRuntimeTrigger Trigger, const FGameplayAbilityTargetDataHandle& TargetData);
    void ApplyRuntimeEffects(const TArray<FEffectConfig>& Effects, const FGameplayAbilityTargetDataHandle& TargetData, float CoefMultiplier);

    // 런타임 트리거 변환 및 타겟 수집 헬퍼 함수들
    static EHeroSkillRuntimeTrigger ConvertRuntimeTriggerFromEventTag(const FGameplayTag& EventTag);
    static void CollectTargetActorsFromTargetData(const FGameplayAbilityTargetDataHandle& TargetData, TArray<AActor*>& OutActors);

    UFUNCTION()
    void OnTaskRuntimeEvent(FGameplayTag EventTag, FGameplayAbilityTargetDataHandle TargetData);

    // Task 콜백 함수들
    UFUNCTION()
    void OnActionCompleted(FGameplayAbilityTargetDataHandle TargetData);

    UFUNCTION()
    void OnActionCancelled(FGameplayAbilityTargetDataHandle TargetData);

private:
    // InstancedPerActor이므로, 활성화마다 초기화
    int32 CurrentActionIndex;                           // 현재 실행 중인 액션의 인덱스

    //SourceObject에서 스킬 데이터 가져와 할당할 변수
    UPROPERTY()
    TObjectPtr<UDataAsset_HeroSkillData> SkillData = nullptr;

    UPROPERTY()
    TArray<FSkillActionRow> RuntimeActionSequence; // 런타임 액션 시퀀스 (SkillData의 ActionSequence를 기반으로, 실행 중에 변경될 수 있음)

    bool bAutoMode = false; // 자동 모드 여부 (자동 모드인 경우 타겟팅 없이 자기 위치 기준으로 액션 실행)
};
