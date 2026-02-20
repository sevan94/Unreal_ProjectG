
#include "AbilitySystem/Abilities/Unit/UnitAbility_BaseMeleeAttack.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "PGGameplayTags.h"
#include "GameplayCueFunctionLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "PGFunctionLibrary.h"

UUnitAbility_BaseMeleeAttack::UUnitAbility_BaseMeleeAttack()
{
    // 기본 설정
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UUnitAbility_BaseMeleeAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    checkf(!MeleeAttackMontages.IsEmpty(), TEXT("MeleeAttackMontages 배열이 비어있습니다!"));

    // 전방에 박스 트레이스를 발사하여 가장 가까운 타겟 액터를 찾음
    FVector StartLocation = GetAvatarActorFromActorInfo()->GetActorLocation();
    FVector EndLocation = StartLocation + GetAvatarActorFromActorInfo()->GetActorForwardVector() * 500.0f;
    TArray<FHitResult> HitResults;

    // 가장 가까운 폰 액터 찾기
    UKismetSystemLibrary::SphereTraceMultiForObjects(this, StartLocation, EndLocation, MeleeAttackDamageRadius, TArray<TEnumAsByte<EObjectTypeQuery>>{ EObjectTypeQuery::ObjectTypeQuery3 /* Pawn */ }, false, TArray<AActor*>(), bEnableTraceDebug ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None, HitResults, true, FLinearColor::Red, FLinearColor::Green, TraceDebugDuration);
    
    // 적대적인 타겟 필터링
    int32 CurrentHitTargets = 0;
    for (FHitResult HitResult : HitResults)
    {
        if (MaxHitTargets <= CurrentHitTargets)
        {
            break;
        }
        if(UPGFunctionLibrary::IsTargetCharacterIsHostile(GetAvatarActorFromActorInfo(), HitResult.GetActor()))
        {
            CachedTargetActors.AddUnique(HitResult.GetActor());
            CurrentHitTargets++;
        }
    }

    // 타겟이 없으면 능력 종료
    if (CurrentHitTargets <= 0)
    {
        EndAbility(Handle, ActorInfo, ActivationInfo, false, true);
        return;
    }

    // 애니메이션 몽타주 재생
    UAnimMontage* SelectedMontage = MeleeAttackMontages[FMath::RandRange(0, MeleeAttackMontages.Num() - 1)];
    UAbilityTask_PlayMontageAndWait* MeleeMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, SelectedMontage);

    // 몽타주 완료 이벤트 바인딩
    if (MeleeMontageTask)
    {
        MeleeMontageTask->OnCompleted.AddUniqueDynamic(this, &UUnitAbility_BaseMeleeAttack::OnMontageFinished);
        MeleeMontageTask->OnInterrupted.AddUniqueDynamic(this, &UUnitAbility_BaseMeleeAttack::OnMontageFinished);
        MeleeMontageTask->OnBlendOut.AddUniqueDynamic(this, &UUnitAbility_BaseMeleeAttack::OnMontageFinished);
        MeleeMontageTask->OnCancelled.AddUniqueDynamic(this, &UUnitAbility_BaseMeleeAttack::OnMontageFinished);

        MeleeMontageTask->ReadyForActivation();
    }

    // 게임플레이 이벤트 대기 태스크 생성
    UAbilityTask_WaitGameplayEvent* MeleeHitEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, PGGameplayTags::Shared_Event_MeleeHit);

    // 이벤트 수신 핸들러 바인딩
    MeleeHitEventTask->EventReceived.AddUniqueDynamic(this, &UUnitAbility_BaseMeleeAttack::HandleApplyDamage);
    MeleeHitEventTask->ReadyForActivation();
}

void UUnitAbility_BaseMeleeAttack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
    CachedTargetActors.Empty();
    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UUnitAbility_BaseMeleeAttack::HandleApplyDamage(FGameplayEventData InEventData)
{
    //// 게임플레이 큐 실행
    //UGameplayCueFunctionLibrary::ExecuteGameplayCueOnActor(GetAvatarActorFromActorInfo(), MeleeAttackCueTag, FGameplayCueParameters());


    // 데미지 적용
    // TODO : 스킬의 데미지 Multiflier를 변수화
    float SkillMultiplierValue = MeleeAttackSkillMultiplier.GetValueAtLevel(GetAbilityLevel());
    FGameplayEffectSpecHandle EffectSpecHandle = MakeOutgoingEffectSpecToTarget(MeleeAttackDamageEffectClass, SkillMultiplierValue);
    for( TWeakObjectPtr<AActor> TargetActor : CachedTargetActors)
    {
        if (TargetActor.IsValid())
        {
            NativeApplyEffectSpecHandleToTarget(TargetActor.Get(), EffectSpecHandle);
        }
    }
}

void UUnitAbility_BaseMeleeAttack::OnMontageFinished()
{
    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}