#include "AbilitySystem/Abilities/Hero/PGHeroSkillGameplayAbility.h"

#include "AbilitySystem/AbilityTasks/SkillAbilityTask_MeleeTrace.h"
#include "AbilitySystem/AbilityTasks/SkillAbilityTask_SpawnActor.h"
#include "AbilitySystem/AbilityTasks/SkillAbilityTask_Buff.h"
#include "Components/Combat/HeroCombatComponent.h"
#include "AbilitySystemComponent.h"
#include "DataAssets/Ability/DataAsset_HeroSkillData.h"
#include "AbilitySystem/Effects/GEffect_Cooldown.h"
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

    if(!CommitCheck(Handle, ActorInfo, ActivationInfo))
    {
        EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
        return;
    }

    SkillData = Cast<UDataAsset_HeroSkillData>(GetCurrentSourceObject());
    if (!SkillData || SkillData->ActionSequence.IsEmpty())
    {
        EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
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

    case ESkillActionType::Buff:
    {
        USkillAbilityTask_Buff* Task = USkillAbilityTask_Buff::Create(this, CurrentAction);
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

    case ESkillActionType::Buff:
    {
        USkillAbilityTask_Buff* Task = USkillAbilityTask_Buff::Create(this, ActionRow, TargetData);
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

void UPGHeroSkillGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
    if (!bWasCancelled)
    {
        CommitAbility(Handle, ActorInfo, ActivationInfo);
    }

    RuntimeActionSequence.Reset();
    SkillData = nullptr;
    CurrentActionIndex = 0;
    bAutoMode = false;
    
    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}


// =========================================================================================
// 쿨타임 관련 로직
// 쿨타임을 Duration만 받아와서 어빌리티 내부에서 이펙트에 SetByCaller로 전달하도록 변경
// 쿨타임 태그도 어빌리티의 태그를 보고 메인 스킬인지 서브 스킬인지 판단해서 적절한 태그를 반환하도록 변경
// =========================================================================================
EHeroSkillType UPGHeroSkillGameplayAbility::GetHeroSkillType() const
{
    const FGameplayAbilitySpec* Spec = GetCurrentAbilitySpec();
    if (!Spec)
    {
        return EHeroSkillType::None;
    }

    const FGameplayTagContainer& DynamicTags = Spec->GetDynamicSpecSourceTags();
    if (DynamicTags.HasTag(PGGameplayTags::Input_ActiveSkill_MainSkill))
    {
        return EHeroSkillType::MainSkill;
    }
    else if (DynamicTags.HasTag(PGGameplayTags::Input_ActiveSkill_SubSkill))
    {
        return EHeroSkillType::SubSkill;
    }
    else
    {
        return EHeroSkillType::None;
    }
}

const FGameplayTagContainer* UPGHeroSkillGameplayAbility::GetCooldownTags() const
{
    static const FGameplayTagContainer MainSkillTagContainer = []()
        {
            FGameplayTagContainer Tags;
            Tags.AddTag(PGGameplayTags::Ability_Cooldown_MainSkill);
            return Tags;
        }();

    static const FGameplayTagContainer SubSkillTagContainer = []()
        {
            FGameplayTagContainer Tags;
            Tags.AddTag(PGGameplayTags::Ability_Cooldown_SubSkill);
            return Tags;
        }();

    // 현재 어빌리티의 태그 종류에 따라 적절한 태그 컨테이너 반환 
    switch (GetHeroSkillType())
    {
    case EHeroSkillType::MainSkill:
        return &MainSkillTagContainer;
    case EHeroSkillType::SubSkill:
        return &SubSkillTagContainer;
    default:
        return nullptr;
    }
}
void UPGHeroSkillGameplayAbility::ApplyCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const
{
    FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(UGEffect_Cooldown::StaticClass());
    
    if (!SpecHandle.IsValid()) return;

    // 슬롯 태그 주입
    switch (GetHeroSkillType())
    {
    case EHeroSkillType::MainSkill:
        SpecHandle.Data->DynamicGrantedTags.AddTag(PGGameplayTags::Ability_Cooldown_MainSkill);
        break;
    case EHeroSkillType::SubSkill:
        SpecHandle.Data->DynamicGrantedTags.AddTag(PGGameplayTags::Ability_Cooldown_SubSkill);
        break;
    default:    
        return;
    }

    // 스킬 데이터에서 쿨타임 Value 가져와서 SetDuration에 설정
    const float CooldownDuration = SkillData ? SkillData->SkillCooldown.GetValueAtLevel(GetAbilityLevel()) : 0.f;

    UE_LOG(LogTemp, Log, TEXT("Applying Cooldown: %f seconds"), CooldownDuration);

    SpecHandle.Data->SetSetByCallerMagnitude(PGGameplayTags::Shared_SetByCaller_Duration, CooldownDuration);

    ApplyGameplayEffectSpecToOwner(Handle, ActorInfo, ActivationInfo, SpecHandle);
}
