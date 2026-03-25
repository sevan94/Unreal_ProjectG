// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Hero/PGHeroSkillGameplayAbility.h"
#include "AbilitySystem/AbilityTasks/SkillAbilityTask_SpawnActor.h"
#include "AbilitySystem/AbilityTasks/SkillAbilityTask_MeleeTrace.h"
#include "Components/Combat/HeroCombatComponent.h"
#include "PGFunctionLibrary.h"
#include "PGGameplayTags.h"
#include "DataAssets/Ability/DataAsset_HeroSkillData.h"
#include "Abilities/GameplayAbilityTargetTypes.h"

namespace HeroSkillRuntimeHelpers
{
    // 스킬 데이터와 런타임 트리거를 기반으로, 런타임 액션 시퀀스 생성
    static void GatherActiveModifiers(
        const UDataAsset_HeroSkillData* SkillData,
        const UAbilitySystemComponent* ASC,
        EHeroSkillRuntimeTrigger Trigger,
        TArray<const FHeroSkillSetBonusModifier*>& OutActive)
    {
        OutActive.Reset();
        if (!SkillData || !ASC) return;

        TArray<const FHeroSkillSetBonusModifier*> NonExclusive;
        TMap<FGameplayTag, const FHeroSkillSetBonusModifier*> HighestTierExclusive; // 동일 세트 중 최고 단계만 적용인 경우, 세트 태그별로 가장 높은 단계의 모디파이어만 저장
    
        for(const FHeroSkillSetBonusModifier& Modifier : SkillData->SetBonusModifiers)
        {
            if(Modifier.Trigger != Trigger) continue; // 트리거 불일치
            const int32 EquippedCount = UPGFunctionLibrary::ResolveSetPieceCount(ASC, Modifier.RequiredSetTag);
            if(EquippedCount < Modifier.RequiredSetPieceCount) continue; // 요구 세트 개수 미달
            if (!Modifier.RequiredSetTag.IsValid()) continue; // 세트 태그 유효하지 않음

            if(!Modifier.bUseHighestTierOnly) // 동일 세트 중 최고 단계만 적용이 아니면 바로 추가
            {
                NonExclusive.Add(&Modifier);
                continue;
            }

            const FHeroSkillSetBonusModifier* const* Found = HighestTierExclusive.Find(Modifier.RequiredSetTag);
            if (!Found || ((*Found)->RequiredSetPieceCount < Modifier.RequiredSetPieceCount))
            {
                HighestTierExclusive.Add(Modifier.RequiredSetTag, &Modifier); // 등록된 규칙이 없거나, 현재 규칙이 더 높은 단계인 경우에만 등록
            }
        }

        OutActive = NonExclusive;
        for (const TPair<FGameplayTag, const FHeroSkillSetBonusModifier*>& Pair : HighestTierExclusive)
        {
            OutActive.Add(Pair.Value);
        }

        OutActive.Sort([](const FHeroSkillSetBonusModifier* A, const FHeroSkillSetBonusModifier* B)
        {
            if (!A || !B) return A != nullptr;
            return A->RequiredSetPieceCount < B->RequiredSetPieceCount;
        });
    }
}


//==================================================================================================================
//==================================================================================================================
//==================================================================================================================

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
    RuntimeActionSequence.Reset();
    
    if(const UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo())
    {
        // 스킬 데이터의 액션 시퀀스를 런타임 액션 시퀀스로 복사
        SkillData->BuildRuntimeActionSequence(ASC, GetAbilityLevel(), RuntimeActionSequence, EHeroSkillRuntimeTrigger::OnSkillStart);
    }

    if (RuntimeActionSequence.IsEmpty())
    {
        RuntimeActionSequence = SkillData->ActionSequence; // 시퀀스를 못 가져왔다면 원본 시퀀스 사용 (빌드 과정에서 조건에 의해 액션이 제거될 수 있기 때문)
    }

    if(RuntimeActionSequence.IsEmpty())
    {
        // 그래도 시퀀스가 비어있다면 어빌리티 종료
        EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
        return;
    }

    CurrentActionIndex = 0;

    // 자동 모드 여부 확인
    UHeroCombatComponent* CombatComp = UPGFunctionLibrary::NativeGetCombatComponentFromActor(GetAvatarActorFromActorInfo());
    bAutoMode = CombatComp ? CombatComp->IsAutoMode() : false;

    ExecuteRuntimeTrigger(EHeroSkillRuntimeTrigger::OnSkillStart, FGameplayAbilityTargetDataHandle()); // 스킬 시작 트리거 실행 (타겟 데이터 없음)

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

    const FSkillActionRow& CurrentAction = RuntimeActionSequence[CurrentActionIndex];

    // Task 생성 => 콜백 바인딩 => Task 실행
    // TODO: 새 액션 추가 시 여기에 케이스 추가
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
        USkillAbilityTask_SpawnActor* Task = USkillAbilityTask_SpawnActor::Create(this, CurrentAction, bAutoMode);
        Task->OnCompleted.AddDynamic(this, &UPGHeroSkillGameplayAbility::OnActionCompleted);
        Task->OnCancelled.AddDynamic(this, &UPGHeroSkillGameplayAbility::OnActionCancelled);
        Task->OnRuntimeEvent.AddDynamic(this, &UPGHeroSkillGameplayAbility::OnTaskRuntimeEvent);
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

void UPGHeroSkillGameplayAbility::OnTaskRuntimeEvent(FGameplayTag EventTag, FGameplayAbilityTargetDataHandle TargetData)
{
    const EHeroSkillRuntimeTrigger Trigger = ConvertRuntimeTriggerFromEventTag(EventTag);
    if(Trigger == EHeroSkillRuntimeTrigger::None) return;

    ExecuteRuntimeTrigger(Trigger, TargetData);
}

EHeroSkillRuntimeTrigger UPGHeroSkillGameplayAbility::ConvertRuntimeTriggerFromEventTag(const FGameplayTag& EventTag)
{
    if (EventTag.MatchesTagExact(PGGameplayTags::Event_Trigger_OnHit))
    {
        return EHeroSkillRuntimeTrigger::OnHit;
    }
    return EHeroSkillRuntimeTrigger::None;
}

void UPGHeroSkillGameplayAbility::ExecuteRuntimeTrigger(EHeroSkillRuntimeTrigger Trigger, const FGameplayAbilityTargetDataHandle& TargetData)
{
    const UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
    if (!SkillData || !ASC) return;

    TArray<const FHeroSkillSetBonusModifier*> ActiveSetBonuses;
    HeroSkillRuntimeHelpers::GatherActiveModifiers(SkillData, ASC, Trigger, ActiveSetBonuses);

    for(const FHeroSkillSetBonusModifier* Modifier : ActiveSetBonuses)
    {
        if (!Modifier) continue;
        const float Coefficient = Modifier->EffectCoefficientMultiplier.GetValueAtLevel(GetAbilityLevel()); // 모든 Effect에 적용할 보정 계수 계산
        
        for (const FHeroSkillRuntimeOp& Op : Modifier->Ops)
        {
            if (Op.OpType != EHeroSkillRuntimeOpType::ApplyEffects) continue;
            ApplyRuntimeEffects(Op.Effects, TargetData, Coefficient);
        }
    }
}

void UPGHeroSkillGameplayAbility::ApplyRuntimeEffects(const TArray<FEffectConfig>& Effects, const FGameplayAbilityTargetDataHandle& TargetData, float CoefMultiplier)
{
    if (Effects.IsEmpty()) return;

    TArray<FEffectConfig> RuntimeEffects = Effects;
    if (!FMath::IsNearlyEqual(CoefMultiplier, 1.f))
    {
        for (FEffectConfig& E : RuntimeEffects)
        {
            const float Mul = E.Multiplier.GetValueAtLevel(GetAbilityLevel());
            E.Multiplier = FScalableFloat(Mul * CoefMultiplier);
        }
    }

    TArray<AActor*> Targets;
    CollectTargetActorsFromTargetData(TargetData, Targets);
    if (Targets.IsEmpty())
    {
        if (AActor* Avatar = GetAvatarActorFromActorInfo()) Targets.Add(Avatar);
    }

    const TArray<FGameplayEffectSpecHandle> SpecHandles = MakeOutgoingEffectSpecsFromEffectConfigs(RuntimeEffects);
    for (AActor* Target : Targets)
    {
        if (!Target) continue;
        for (const FGameplayEffectSpecHandle& SpecHandle : SpecHandles)
        {
            if (SpecHandle.IsValid())
            {
                NativeApplyEffectSpecHandleToTarget(Target, SpecHandle);
            }
        }
    }
}

void UPGHeroSkillGameplayAbility::CollectTargetActorsFromTargetData(const FGameplayAbilityTargetDataHandle& TargetData, TArray<AActor*>& OutActors)
{
    OutActors.Reset();
    TSet<AActor*> UniqueActors;

    for (int32 i = 0; i < TargetData.Num(); ++i)
    {
        const FGameplayAbilityTargetData* Data = TargetData.Get(i);
        if (!Data) continue;

        if (const FHitResult* Hit = Data->GetHitResult())
        {
            if (AActor* HitActor = Hit->GetActor()) UniqueActors.Add(HitActor);
        }

        TArray<TWeakObjectPtr<AActor>> Actors;
        Actors = Data->GetActors();
        for (const TWeakObjectPtr<AActor>& WeakActor : Actors)
        {
            if (AActor* Actor = WeakActor.Get()) UniqueActors.Add(Actor);
        }
    }

    for (AActor* Actor : UniqueActors) OutActors.Add(Actor);
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
    RuntimeActionSequence.Reset();
    SkillData = nullptr;
    CurrentActionIndex = 0;
    bAutoMode = false;

    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}