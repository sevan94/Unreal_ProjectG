// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Player/HeroAbility_PointTargetAOE.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Abilities/Tasks/AbilityTask_WaitTargetData.h"
#include "AbilitySystem/TargetActor/GATargetActor_AOEGroundTrace.h"
#include "PGGameplayTags.h"
#include "Actors/AOESkillActor.h"

UHeroAbility_PointTargetAOE::UHeroAbility_PointTargetAOE()
{
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UHeroAbility_PointTargetAOE::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
    {
        EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
        return;
    }

    // WaitTargetData 태스크로 GATargetActor_AOEGroundTrace 액터를 스폰하고 타겟 데이터를 기다립니다.
    UAbilityTask_WaitTargetData* WaitTargetDataTask = UAbilityTask_WaitTargetData::WaitTargetData(
        this, NAME_None, EGameplayTargetingConfirmation::UserConfirmed, AGATargetActor_AOEGroundTrace::StaticClass());

    WaitTargetDataTask->ValidData.AddUniqueDynamic(this, &UHeroAbility_PointTargetAOE::OnTargetDataReady);
    WaitTargetDataTask->Cancelled.AddUniqueDynamic(this, &UHeroAbility_PointTargetAOE::OnTargetDataCancelled);

    // BeginSpawningActor / FinishSpawningActor 패턴을 사용하여 타겟 액터를 스폰합니다.
    AGameplayAbilityTargetActor* SpawnedTargetActor = nullptr;
    if (WaitTargetDataTask->BeginSpawningActor(this, AGATargetActor_AOEGroundTrace::StaticClass(), SpawnedTargetActor))
    {
        // 타겟 액터 초기화 (예: 범위 설정)
        if (AGATargetActor_AOEGroundTrace* GroundTraceTargetActor = Cast<AGATargetActor_AOEGroundTrace>(SpawnedTargetActor))
        {
            float AOERadius = 0.f;
            if (AOEConfig.SpawnedActorClass)
            {
                if (AAOESkillActor* CDO = AOEConfig.SpawnedActorClass->GetDefaultObject<AAOESkillActor>())
                {
                    AOERadius = CDO->GetAOERadius();
                }
            }

            GroundTraceTargetActor->PreviewRadius = AOERadius;
            GroundTraceTargetActor->PreviewDecalSize = AOERadius;
            GroundTraceTargetActor->AOETraceDecalMaterial = AOEConfig.AOEIndicatorDecalMaterial ? AOEConfig.AOEIndicatorDecalMaterial.Get() : nullptr;
            GroundTraceTargetActor->OwnerActor = GetAvatarActorFromActorInfo();
        }

        WaitTargetDataTask->FinishSpawningActor(this, SpawnedTargetActor);
    }

    WaitTargetDataTask->ReadyForActivation();
}

void UHeroAbility_PointTargetAOE::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
    CachedHitLocation = FVector::ZeroVector;

    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UHeroAbility_PointTargetAOE::OnTargetDataReady(const FGameplayAbilityTargetDataHandle& InTargetDataHandle)
{
    if (!InTargetDataHandle.IsValid(0))
    {
        EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
        return;
    }

    // GroundTrace는 HitResult를 포함한 SingleTargetHit 필터로 위치를 가져온다.
    if (const FGameplayAbilityTargetData* TargetData = InTargetDataHandle.Get(0))
    {
        const FHitResult* HitResult = TargetData->GetHitResult();
        CachedHitLocation = HitResult ? HitResult->Location : FVector::ZeroVector;
    }

    // 몽타주가 없으면 즉시 스폰하고 종료
    if (!AOEConfig.Montage.IsValid())
    {
        SpawnAndInitializeAOEActor(CachedHitLocation);
        EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
        return;
    }

    // 몽타주 재생 태스크
    UAbilityTask_PlayMontageAndWait* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, AOEConfig.Montage.Get());
    if (MontageTask)
    {
        MontageTask->OnCompleted.AddUniqueDynamic(this, &UHeroAbility_PointTargetAOE::OnAOEMontageFinished);
        MontageTask->OnInterrupted.AddUniqueDynamic(this, &UHeroAbility_PointTargetAOE::OnAOEMontageFinished);
        MontageTask->OnBlendOut.AddUniqueDynamic(this, &UHeroAbility_PointTargetAOE::OnAOEMontageFinished);
        MontageTask->OnCancelled.AddUniqueDynamic(this, &UHeroAbility_PointTargetAOE::OnAOEMontageFinished);
        
        MontageTask->ReadyForActivation();
    }

    // 이벤트 실행 대기 태스크
    // 애님 노티파이에서 전송
    UAbilityTask_WaitGameplayEvent* WaitEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, PGGameplayTags::Shared_Event_AOEExecute);

    WaitEventTask->EventReceived.AddUniqueDynamic(this, &UHeroAbility_PointTargetAOE::OnApplyAOEDamage);
    WaitEventTask->ReadyForActivation();
}

void UHeroAbility_PointTargetAOE::OnApplyAOEDamage(FGameplayEventData EventData)
{
    SpawnAndInitializeAOEActor(CachedHitLocation);
}


void UHeroAbility_PointTargetAOE::OnTargetDataCancelled(const FGameplayAbilityTargetDataHandle& InTargetDataHandle)
{
    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UHeroAbility_PointTargetAOE::OnAOEMontageFinished()
{
    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}



//void UHeroAbility_PointTargetAOE::OnApplyAOEDamage(FGameplayEventData EventData)
//{
//    FGameplayCueParameters GameplayCueParameters;
//    GameplayCueParameters.Location = CachedHitLocation;
//    UGameplayCueFunctionLibrary::ExecuteGameplayCueOnActor(GetAvatarActorFromActorInfo(), AOEAttackConfig.ImpactCueTag, GameplayCueParameters);
//
//
//    // 무시할 액터 설정
//    TArray<AActor*> IgnoredActors;
//    IgnoredActors.Add(GetAvatarActorFromActorInfo());
//    // 히트 위치 주변의 액터 오버랩 검사
//    UKismetSystemLibrary::SphereOverlapActors(
//        this,
//        CachedHitLocation,
//        AOEAttackConfig.AOEAttackRadius,
//        TArray<TEnumAsByte<EObjectTypeQuery>>{ EObjectTypeQuery::ObjectTypeQuery3 }, // Pawn 객체 타입
//        AUnitCharacter::StaticClass(),
//        IgnoredActors,
//        HitActors
//    );
//
//    float SkillMultiplierValue = AOEAttackConfig.SkillMultiplier.GetValueAtLevel(GetAbilityLevel());
//    FGameplayEffectSpecHandle EffectSpecHandle = MakeHeroDamageEffectSpecHandle(AOEAttackConfig.DamageEffectClass.Get(), SkillMultiplierValue);
//
//    for (AActor* HitActor : HitActors)
//    {
//        if (UPGFunctionLibrary::IsTargetCharacterHostile(GetAvatarActorFromActorInfo(), HitActor))
//        {
//            NativeApplyEffectSpecHandleToTarget(HitActor, EffectSpecHandle);
//            UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(HitActor, PGGameplayTags::Shared_Event_HitReact, FGameplayEventData());
//        }
//    }
//}
