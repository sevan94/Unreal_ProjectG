#include "AbilitySystem/Abilities/Hero/PGHeroSkillGameplayAbility.h"

#include "AbilitySystem/AbilityTasks/SkillAbilityTask_MeleeTrace.h"
#include "AbilitySystem/AbilityTasks/SkillAbilityTask_SpawnActor.h"
#include "Components/Combat/HeroCombatComponent.h"
#include "DataAssets/Ability/DataAsset_HeroSkillData.h"
#include "PGFunctionLibrary.h"
#include "PGGameplayTags.h"

UPGHeroSkillGameplayAbility::UPGHeroSkillGameplayAbility()
{
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerExecution;
}

void UPGHeroSkillGameplayAbility::ActivateAbility(
    const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo,
    const FGameplayEventData* TriggerEventData)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    SkillData = Cast<UDataAsset_HeroSkillData>(GetCurrentSourceObject());
    if (!SkillData || SkillData->ActionSequence.IsEmpty())
    {
        EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
        return;
    }

    RuntimeActionSequence.Reset();

    if (const UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo())
    {
        SkillData->BuildRuntimeActionSequence(ASC, GetAbilityLevel(), RuntimeActionSequence);
    }

    if (RuntimeActionSequence.IsEmpty())
    {
        RuntimeActionSequence = SkillData->ActionSequence;
    }

    if (RuntimeActionSequence.IsEmpty())
    {
        EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
        return;
    }

    CurrentActionIndex = 0;

    UHeroCombatComponent* CombatComp = UPGFunctionLibrary::NativeGetCombatComponentFromActor(GetAvatarActorFromActorInfo());
    bAutoMode = CombatComp ? CombatComp->IsAutoMode() : false;

    ExecuteNextAction();
}

void UPGHeroSkillGameplayAbility::ExecuteNextAction()
{
    if (!RuntimeActionSequence.IsValidIndex(CurrentActionIndex))
    {
        EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), false, false);
        return;
    }

    const FSkillActionRow& CurrentAction = RuntimeActionSequence[CurrentActionIndex];

    switch (CurrentAction.ActionType)
    {
    case ESkillActionType::MeleeTrace:
    {
        USkillAbilityTask_MeleeTrace* Task = USkillAbilityTask_MeleeTrace::Create(this, CurrentAction);
        Task->OnCompleted.AddDynamic(this, &UPGHeroSkillGameplayAbility::OnActionCompleted);
        Task->OnCancelled.AddDynamic(this, &UPGHeroSkillGameplayAbility::OnActionCancelled);
        Task->OnRuntimeEvent.AddDynamic(this, &UPGHeroSkillGameplayAbility::OnTaskRuntimeEvent);
        Task->ReadyForActivation();
        break;
    }

    case ESkillActionType::SpawnActor:
    {
        // SpawnActor 계열 반응은 SkillActor 내부에서 처리하므로 Ability로 RuntimeEvent를 올리지 않는다.
        USkillAbilityTask_SpawnActor* Task = USkillAbilityTask_SpawnActor::Create(
            this,
            CurrentAction,
            bAutoMode,
            FGameplayAbilityTargetDataHandle(),
            false);
        Task->OnCompleted.AddDynamic(this, &UPGHeroSkillGameplayAbility::OnActionCompleted);
        Task->OnCancelled.AddDynamic(this, &UPGHeroSkillGameplayAbility::OnActionCancelled);
        Task->ReadyForActivation();
        break;
    }

    default:
        ++CurrentActionIndex;
        ExecuteNextAction();
        break;
    }
}

bool UPGHeroSkillGameplayAbility::ConvertEventTagToTrigger(const FGameplayTag& EventTag, EHeroSkillEventTrigger& OutTrigger)
{
    if (EventTag.MatchesTagExact(PGGameplayTags::Shared_Event_MeleeHit) || EventTag.MatchesTagExact(PGGameplayTags::Event_Trigger_OnHit))
    {
        OutTrigger = EHeroSkillEventTrigger::OnHit;
        return true;
    }

    if (EventTag.MatchesTagExact(PGGameplayTags::Event_Trigger_OnKill))
    {
        OutTrigger = EHeroSkillEventTrigger::OnKill;
        return true;
    }

    return false;
}

void UPGHeroSkillGameplayAbility::OnTaskRuntimeEvent(FGameplayTag EventTag, FGameplayAbilityTargetDataHandle TargetData)
{
    EHeroSkillEventTrigger Trigger = EHeroSkillEventTrigger::OnHit;
    if (!ConvertEventTagToTrigger(EventTag, Trigger))
    {
        return;
    }

    ExecuteEventTrigger(Trigger, EventTag, TargetData);
}

void UPGHeroSkillGameplayAbility::ExecuteEventTrigger(EHeroSkillEventTrigger Trigger, const FGameplayTag& SourceEventTag, const FGameplayAbilityTargetDataHandle& TargetData)
{
    if (!SkillData)
    {
        return;
    }

    const UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
    if (!ASC)
    {
        return;
    }

    TArray<FSkillActionRow> EventActions;
    SkillData->BuildRuntimeEventActionSequence(ASC, GetAbilityLevel(), EventActions, Trigger, SourceEventTag);

    for (const FSkillActionRow& ActionRow : EventActions)
    {
        ExecuteEventAction(ActionRow, TargetData);
    }
}

void UPGHeroSkillGameplayAbility::ExecuteEventAction(const FSkillActionRow& ActionRow, const FGameplayAbilityTargetDataHandle& TargetData)
{
    switch (ActionRow.ActionType)
    {
    case ESkillActionType::SpawnActor:
    {
        USkillAbilityTask_SpawnActor* Task = USkillAbilityTask_SpawnActor::Create(
            this,
            ActionRow,
            bAutoMode,
            TargetData,
            false);
        Task->ReadyForActivation();
        break;
    }

    case ESkillActionType::MeleeTrace:
    {
        USkillAbilityTask_MeleeTrace* Task = USkillAbilityTask_MeleeTrace::Create(this, ActionRow);
        Task->ReadyForActivation();
        break;
    }

    default:
        break;
    }
}

void UPGHeroSkillGameplayAbility::OnActionCompleted(FGameplayAbilityTargetDataHandle TargetData)
{
    ++CurrentActionIndex;
    ExecuteNextAction();
}

void UPGHeroSkillGameplayAbility::OnActionCancelled(FGameplayAbilityTargetDataHandle TargetData)
{
    EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), false, true);
}

void UPGHeroSkillGameplayAbility::EndAbility(
    const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo,
    bool bReplicateEndAbility,
    bool bWasCancelled)
{
    RuntimeActionSequence.Reset();
    SkillData = nullptr;
    CurrentActionIndex = 0;
    bAutoMode = false;

    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
