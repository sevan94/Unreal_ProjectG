#include "DataAssets/Ability/DataAsset_HeroSkillData.h"
#include "AbilitySystemComponent.h"
#include "PGFunctionLibrary.h"

void UDataAsset_HeroSkillData::BuildRuntimeActionSequence(const UAbilitySystemComponent* InASC, int32 InAbilityLevel, TArray<FSkillActionRow>& OutRuntimeActions, EHeroSkillRuntimeTrigger InTrigger) const
{
    OutRuntimeActions = ActionSequence; // 기본 액션 시퀀스 복사
    if (OutRuntimeActions.IsEmpty()) return;

    TArray<const FHeroSkillSetBonusModifier*> ActiveNoneExclusive; // 동일 세트 최고 단계만 적용하는 규칙이 아닌, 또는 요구 세트 태그가 유효하지 않은 규칙을 저장할 배열
    TMap<FGameplayTag, const FHeroSkillSetBonusModifier*> ActiveHighestTierBySet; // 요구 세트 태그별로, 동일 세트 최고 단계만 적용하는 규칙 중 가장 높은 단계의 규칙을 저장할 맵
    
    for (const FHeroSkillSetBonusModifier& Modifier : SetBonusModifiers)
    {
        if (Modifier.Trigger != InTrigger) continue; // 트리거 조건에 맞지 않는 규칙은 무시

        const int32 CurrentPiece = UPGFunctionLibrary::ResolveSetPieceCount(InASC, Modifier.RequiredSetTag);
        if (CurrentPiece < Modifier.RequiredSetPieceCount) continue; // 요구 세트 개수 조건을 충족하지 못하는 규칙은 무시
		if (!Modifier.RequiredSetTag.IsValid()) continue; // 요구 세트 태그가 유효하지 않은 규칙은 무시

        if (!Modifier.bUseHighestTierOnly)
        {
            ActiveNoneExclusive.Add(&Modifier); // 동일 세트 최고 단계만 적용하지 않는 규칙은 무조건 추가
            continue;
        }

        const FHeroSkillSetBonusModifier* const* Found = ActiveHighestTierBySet.Find(Modifier.RequiredSetTag); // 동일 세트 최고 단계만 적용하는 규칙인 경우, 해당 세트 태그에 이미 등록된 규칙이 있는지 확인
        if (!Found || ((*Found)->RequiredSetPieceCount < Modifier.RequiredSetPieceCount))
        {
            ActiveHighestTierBySet.Add(Modifier.RequiredSetTag, &Modifier); // 등록된 규칙이 없거나, 현재 규칙이 더 높은 단계인 경우에만 등록
        }
    }

    TArray<const FHeroSkillSetBonusModifier*> ActiveModifiers = ActiveNoneExclusive; // 최종적으로 적용할 규칙 배열 초기화
    for(const TPair<FGameplayTag, const FHeroSkillSetBonusModifier*>& Pair : ActiveHighestTierBySet)
    {
        ActiveModifiers.Add(Pair.Value); // 동일 세트 최고 단계만 적용하는 규칙 중에서 가장 높은 단계의 규칙을 최종적으로 적용할 규칙 배열에 추가
    }

    ActiveModifiers.Sort([](const FHeroSkillSetBonusModifier& A, const FHeroSkillSetBonusModifier& B)
        {
            return A.RequiredSetPieceCount < B.RequiredSetPieceCount; // 요구 세트 개수 기준 오름 정렬 (개수가 적은 규칙이 먼저 적용되도록)
        });

    for (const FHeroSkillSetBonusModifier* Modifier : ActiveModifiers)
    {
        if (!Modifier) continue;

        const float CoefMultiplier = Modifier->EffectCoefficientMultiplier.GetValueAtLevel(InAbilityLevel);
        MultiplyAllEffectMultipliers(OutRuntimeActions, CoefMultiplier); // 보정 계수만큼 모든 액션의 효과 계수를 곱해주는 함수 호출
        
        for (const FHeroSkillRuntimeOp& Op : Modifier->Ops)
        {
            if(!IsOpSupportedByTrigger(InTrigger, Op.OpType)) continue; // 이 트리거에서 지원되지 않는 오퍼레이션은 무시

            switch (Op.OpType)
            {
            case EHeroSkillRuntimeOpType::AddAction:
                if (OutRuntimeActions.Num() < MaxRuntimeActionCount)
                {
                    OutRuntimeActions.Add(Op.ActionToInject); // 액션 추가
                }
                break;

            case EHeroSkillRuntimeOpType::InsertActionBefore:
            {
                const int32 TargetIndex = FindUniqueActionIndexById(OutRuntimeActions, Op.TargetActionID);
                if (TargetIndex != INDEX_NONE && OutRuntimeActions.Num() < MaxRuntimeActionCount)
                {
                    OutRuntimeActions.Insert(Op.ActionToInject, TargetIndex); // 액션 삽입(앞)
                }
                break;
            }

            case EHeroSkillRuntimeOpType::InsertActionAfter:
            {
                const int32 TargetIndex = FindUniqueActionIndexById(OutRuntimeActions, Op.TargetActionID);
                if (TargetIndex != INDEX_NONE && OutRuntimeActions.Num() < MaxRuntimeActionCount)
                {
                    OutRuntimeActions.Insert(Op.ActionToInject, TargetIndex + 1); // 액션 삽입(뒤)
                }
                break;
            }
            case EHeroSkillRuntimeOpType::OverrideAction:
            {
                const int32 TargetIndex = FindUniqueActionIndexById(OutRuntimeActions, Op.TargetActionID);
                if (TargetIndex != INDEX_NONE)
                {
                    FSkillActionRow Overridden = Op.OverrideActionRow;
                    if (Overridden.ActionID.IsNone())
                    {
                        Overridden.ActionID = OutRuntimeActions[TargetIndex].ActionID;
                    }
                    OutRuntimeActions[TargetIndex] = Overridden;
                }
                break;
            }

            case EHeroSkillRuntimeOpType::OverrideActionValue:
            {
                const int32 TargetIndex = FindUniqueActionIndexById(OutRuntimeActions, Op.TargetActionID);
                if (TargetIndex != INDEX_NONE)
                {
                    const float Value = Op.ActionScalarValue.GetValueAtLevel(InAbilityLevel);
                    OverrideActionScalar(OutRuntimeActions[TargetIndex], Op.ActionScalarField, Value); // 액션 값 덮어쓰기
                }
                break;
            }

            case EHeroSkillRuntimeOpType::OverrideEffectValue:
            {
                const int32 TargetIndex = FindUniqueActionIndexById(OutRuntimeActions, Op.TargetActionID);
                if (TargetIndex != INDEX_NONE)
                {
                    if (TArray<FEffectConfig>* Effects = GetMutableEffects(OutRuntimeActions[TargetIndex]))
                    {
                        const float Value = Op.EffectScalarValue.GetValueAtLevel(InAbilityLevel);
                        OverrideEffectScalar(*Effects, Op.EffectIndex, Op.EffectScalarField, Value); // 이펙트 값 덮어쓰기
                    }
                }
                break;
            }

            case EHeroSkillRuntimeOpType::ApplyEffects:
                break; // 이펙트 적용은 런타임에 처리되어야 하므로, 스킬 데이터 단계에서는 처리하지 않음

            default:
                break;
            }
        }
    }

    if(OutRuntimeActions.Num() > MaxRuntimeActionCount)
    {
        OutRuntimeActions.SetNum(MaxRuntimeActionCount); // 최대 런타임 액션 수를 초과하는 경우, 초과분 제거
    }
}

int32 UDataAsset_HeroSkillData::FindUniqueActionIndexById(const TArray<FSkillActionRow>& Actions, const FName& ActionId)
{
    if (ActionId.IsNone()) return INDEX_NONE;

    int32 FoundIndex = INDEX_NONE;
    for(int32 i = 0; i< Actions.Num(); ++i)
    {
        if (Actions[i].ActionID == ActionId)
        {
            if (FoundIndex != INDEX_NONE)
            {
                return INDEX_NONE; // 중복된 ID 발견 시, 유효하지 않은 인덱스 반환
            }
            FoundIndex = i;
        }
    }
    return FoundIndex;
}

TArray<FEffectConfig>* UDataAsset_HeroSkillData::GetMutableEffects(FSkillActionRow& Action)
{
    switch (Action.ActionType)
    {
    case ESkillActionType::SpawnActor:
        return &Action.SpawnableConfig.Effects;
    case ESkillActionType::MeleeTrace:
        return &Action.MeleeTraceConfig.Effects;
    default:
        return nullptr;
    }
}

void UDataAsset_HeroSkillData::OverrideActionScalar(FSkillActionRow& Action, EHeroSkillActionScalarField Field, float Value)
{
    if (Action.ActionType == ESkillActionType::SpawnActor)
    {
        switch (Field)
        {
        case EHeroSkillActionScalarField::SpawnRadius:
            Action.SpawnableConfig.Radius = Value;
            return;
        case EHeroSkillActionScalarField::SpawnLifeSpan:
            Action.SpawnableConfig.LifeSpan = Value;
            return;
        case EHeroSkillActionScalarField::SpawnHtisPerLifeSpan:
            Action.SpawnableConfig.HitsPerLifeSpan = Value;
            return;
        default:
            return;
        }
    }

    if(Action.ActionType == ESkillActionType::MeleeTrace)
    {
        switch (Field)
        {
        case EHeroSkillActionScalarField::TraceRadius:
            Action.MeleeTraceConfig.Radius = Value;
            return;
        case EHeroSkillActionScalarField::TraceOffsetRange:
            Action.MeleeTraceConfig.TraceOffsetRange = Value;
            return;
        case EHeroSkillActionScalarField::TraceMaxHit:
            Action.MeleeTraceConfig.MaxHit = FMath::Max(1, FMath::RoundToInt(Value));
            return;
        default:
            return;
        }
    }
}

void UDataAsset_HeroSkillData::OverrideEffectScalar(TArray<FEffectConfig>& Effects, int32 EffectIndex, EHeroSkillEffectScalarField Field, float Value)
{
    auto ApplyScalar = [&](FEffectConfig& Effect)
    {
        switch (Field)
        {
        case EHeroSkillEffectScalarField::EffectMultiplier:
            Effect.Multiplier = FScalableFloat(Value);
            break;
        case EHeroSkillEffectScalarField::EffectBaseAmount:
            Effect.BaseAmount = FScalableFloat(Value);
            break;
        case EHeroSkillEffectScalarField::EffectDuration:
            Effect.Duration = FScalableFloat(Value);
            break;
        default:
            break;
        }
    };

    if (EffectIndex < 0)
    {
        // EffectIndex가 음수이면, 모든 이펙트에 대해 적용
        for (FEffectConfig& Effect : Effects)
        {
            ApplyScalar(Effect);
        }
        return;
    }

    if (Effects.IsValidIndex(EffectIndex))
    {
        ApplyScalar(Effects[EffectIndex]);
    }
}

void UDataAsset_HeroSkillData::MultiplyAllEffectMultipliers(TArray<FSkillActionRow>& Actions, float MultiplierAtLevel)
{
    if (FMath::IsNearlyEqual(MultiplierAtLevel, 1.f)) return;

    for (FSkillActionRow& Action : Actions)
    {
        TArray<FEffectConfig>* Effects = GetMutableEffects(Action);
        if (!Effects) continue;

        for (FEffectConfig& Effect : *Effects)
        {
            const float Current = Effect.Multiplier.GetValueAtLevel(1.f); // TODO: 레벨에 따른 보정이 필요한 경우, 적절한 레벨을 전달하도록 수정
            Effect.Multiplier = FScalableFloat(Current * MultiplierAtLevel);
        }
    }
}

bool UDataAsset_HeroSkillData::IsOpSupportedByTrigger(EHeroSkillRuntimeTrigger Trigger, EHeroSkillRuntimeOpType OpType)
{
    // 시작 시점: 시퀀스 패치 + 즉시 실행형 모두 허용
    if (Trigger == EHeroSkillRuntimeTrigger::OnSkillStart)
    {
        return true;
    }

    // 실행 중 트리거: 실행형만 허용
    switch (Trigger)
    {
    case EHeroSkillRuntimeTrigger::OnHit:
    case EHeroSkillRuntimeTrigger::OnKill:
    case EHeroSkillRuntimeTrigger::OnSkillEnd:
        return OpType == EHeroSkillRuntimeOpType::ApplyEffects;
    default:
        return false;
    }
}
