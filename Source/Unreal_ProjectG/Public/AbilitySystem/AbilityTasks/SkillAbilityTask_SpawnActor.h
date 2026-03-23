// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/AbilityTasks/SkillAbilityTask.h"
#include "DataAssets/Ability/DataAsset_HeroSkillData.h"
#include "SkillAbilityTask_SpawnActor.generated.h"


class ASkillActor;

/**
 * 1단계 : WaitTargetData로 위치 확보
 * 2단계 : 몽타주에서 Notify로 SpawnActor 이벤트 수신
 * 3단계 : 실제 액터 소환
 * 
 * 타깃 정책이 AtTargetPoint인 경우 WaitTargetData로 위치 확보, 그렇지 않은 경우 자기 위치를 기준
 */
UCLASS()
class UNREAL_PROJECTG_API USkillAbilityTask_SpawnActor : public USkillAbilityTask
{
	GENERATED_BODY()
	
public:
    static USkillAbilityTask_SpawnActor* Create(
        UGameplayAbility* OwningAbility,
        const FSkillActionRow& ActionRow,
        bool bIsAutoMode,
        const FGameplayAbilityTargetDataHandle& InPreviousTargetData);

    virtual void Activate() override;
    virtual void OnDestroy(bool bInOwnerFinished) override;

private:
    // =================================================
    // 1단계 : 위치 확보
    // =================================================
    void StartWaitTargetData();

    UFUNCTION()
    void OnTargetDataReady(const FGameplayAbilityTargetDataHandle& TargetDataHandle);

    UFUNCTION()
    void OnTargetDataCancelled(const FGameplayAbilityTargetDataHandle& TargetDataHandle);

    // TargetPolicy에 맞춰서 위치 계산
    FVector ResolveAutoTargetLocation() const;

    // =================================================
    // 2단계 : 몽타주에서 Notify로 SpawnActor 이벤트 수신
    // =================================================
    void PlayMontageOrSpawn();

    UFUNCTION()
    void OnMontageCancelled();

    UFUNCTION()
    void OnSpawnEventReceived(FGameplayEventData Payload);      // AnimNotify

    // =================================================
    // 3단계: 액터 소환 및 파괴 대기
    // =================================================
    // 실제 소환 처리
    void SpawnActorAtLocation(const FVector& Location, const FRotator& Rotation);
    
    // 액터 파괴 이벤트
    UFUNCTION()
    void OnActorDestroyedEvent(FGameplayEventData Payload);

private:
    FSkillActionRow CachedActionRow;
    bool bAutoMode = false;

    // 소환 위치 및 회전
    FVector CachedSpawnLocation = FVector::ZeroVector;
    FRotator CachedSpawnRotation = FRotator::ZeroRotator;

    // 소환된 액터 참조
    // 정리 용도
    TWeakObjectPtr<ASkillActor> SpawnedActor;

    FGameplayAbilityTargetDataHandle PreviousTargetDataHandle; // WaitTargetData로 확보한 위치 정보 캐싱
};
