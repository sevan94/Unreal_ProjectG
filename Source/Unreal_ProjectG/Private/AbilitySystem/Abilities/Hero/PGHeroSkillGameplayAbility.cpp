// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Hero/PGHeroSkillGameplayAbility.h"
#include "AbilitySystem/AbilityTasks/SkillAbilityTask_SpawnActor.h"
#include "AbilitySystem/AbilityTasks/SkillAbilityTask_MeleeTrace.h"
#include "Components/Combat/HeroCombatComponent.h"
#include "PGFunctionLibrary.h"
#include "PGGameplayTags.h"

UPGHeroSkillGameplayAbility::UPGHeroSkillGameplayAbility()
{
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerExecution;
}

void UPGHeroSkillGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    SkillData = Cast<UDataAsset_HeroSkillData>(GetCurrentSourceObject());

    // 스킬 데이터가 없거나, 액션 시퀀스가 비어있다면 어빌리티 종료
    if (!SkillData || SkillData->ActionSequence.IsEmpty())
    {
        EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
        return;
    }

    // InstancedPerActor이므로, 활성화마다 초기화
    CurrentActionIndex = 0;

    // 자동 모드 여부 확인
    UHeroCombatComponent* CombatComp = UPGFunctionLibrary::NativeGetCombatComponentFromActor(GetAvatarActorFromActorInfo());
    bAutoMode = CombatComp ? CombatComp->IsAutoMode() : false;

    ExecuteNextAction();
}

// 실행 루프
// GA는 Task 생성과 콜백 바인딩만 담당, 실제 로직은 Task에서 구현
void UPGHeroSkillGameplayAbility::ExecuteNextAction()
{
    if (!SkillData->ActionSequence.IsValidIndex(CurrentActionIndex))
    {
        // 모든 액션 완료 -> 어빌리티 정상 종료
        EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), false, false);
        return;
    }

    const FSkillActionRow& CurrentAction = SkillData->ActionSequence[CurrentActionIndex];

    // Task 생성 => 콜백 바인딩 => Task 실행
    // TODO: 새 액션 추가 시 여기에 케이스 추가
    switch (CurrentAction.ActionType)
    {
    case ESkillActionType::MeleeTrace:
    {
        auto* Task = USkillAbilityTask_MeleeTrace::Create(this, CurrentAction);
        Task->OnCompleted.AddDynamic(this, &UPGHeroSkillGameplayAbility::OnActionCompleted);
        Task->OnCancelled.AddDynamic(this, &UPGHeroSkillGameplayAbility::OnActionCancelled);
        Task->ReadyForActivation();
        break;
    }
    case ESkillActionType::SpawnActor:
    {
        auto* Task = USkillAbilityTask_SpawnActor::Create(this, CurrentAction, bAutoMode);
        Task->OnCompleted.AddDynamic(this, &UPGHeroSkillGameplayAbility::OnActionCompleted);
        Task->OnCancelled.AddDynamic(this, &UPGHeroSkillGameplayAbility::OnActionCancelled);
        Task->ReadyForActivation();
        break;
    }
    default:
        // 알수 없는 액션 타입
        CurrentActionIndex++;
        ExecuteNextAction();
        break;
    }
}

void UPGHeroSkillGameplayAbility::OnActionCompleted(FGameplayAbilityTargetDataHandle TargetData)
{
    // Task에서 타겟 데이터를 받아와서 저장
    // 다음 액션에서 CurrentTargetData 활용 가능
    CurrentActionIndex++;
    ExecuteNextAction();
}

void UPGHeroSkillGameplayAbility::OnActionCancelled(FGameplayAbilityTargetDataHandle TargetData)
{
    EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), false, true);
}

void UPGHeroSkillGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}