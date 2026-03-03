// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Player/HeroAbility_BaseMeleeAttack.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Components/Combat/HeroCombatComponent.h"
#include "PGGameplayTags.h"
#include "GameplayCueFunctionLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Types/PGEnumTypes.h"
#include "TimerManager.h"
#include "Abilities/Tasks/AbilityTask_WaitDelay.h"
#include "PGFunctionLibrary.h"
#include "AbilitySystemBlueprintLibrary.h"

#include "DataAssets/Ability/AbilityConfig.h"

UHeroAbility_BaseMeleeAttack::UHeroAbility_BaseMeleeAttack()
{
    // 기본 설정
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UHeroAbility_BaseMeleeAttack::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
    Super::OnGiveAbility(ActorInfo, Spec);

    UMeleeAttackAbilityConfig* Data = Cast<UMeleeAttackAbilityConfig>(GetCurrentAbilitySpec()->SourceObject.Get());
    if (Data)
    {
        MeleeAttackMontage = Data->AbilityMontage;
        MeleeAttackSkillMultiplier = Data->DamageMultiplier;
    }
}

void UHeroAbility_BaseMeleeAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
    {
        EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
        return;
    }

    if (CachedWeaponStaticMesh == nullptr)
    {
        CachedWeaponStaticMesh = GetHeroCombatComponentFromActorInfo()->CachedWeaponMeshComponent.Get();
    }

    checkf(MeleeAttackMontage, TEXT("MeleeAttackMontage가 비어있습니다!"));

    // 애니메이션 몽타주 재생
    UAbilityTask_PlayMontageAndWait* MeleeMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, MeleeAttackMontage);
    
    // 몽타주 완료 이벤트 바인딩
    if (MeleeMontageTask)
    {
        MeleeMontageTask->OnCompleted.AddUniqueDynamic(this, &UHeroAbility_BaseMeleeAttack::OnMontageFinished);
        MeleeMontageTask->OnInterrupted.AddUniqueDynamic(this, &UHeroAbility_BaseMeleeAttack::OnMontageFinished);
        MeleeMontageTask->OnBlendOut.AddUniqueDynamic(this, &UHeroAbility_BaseMeleeAttack::OnMontageFinished);
        MeleeMontageTask->OnCancelled.AddUniqueDynamic(this, &UHeroAbility_BaseMeleeAttack::OnMontageFinished);

        MeleeMontageTask->ReadyForActivation();
    }

    // 게임플레이 이벤트 대기 태스크 생성
    UAbilityTask_WaitGameplayEvent* MeleeHitEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, PGGameplayTags::Shared_Event_AttackTraceToggle);

    // 이벤트 수신 핸들러 바인딩
    MeleeHitEventTask->EventReceived.AddUniqueDynamic(this, &UHeroAbility_BaseMeleeAttack::ToggleWeaponTrace);
    MeleeHitEventTask->ReadyForActivation();

    UE_LOG(LogTemp, Log, TEXT("Hero Melee Attack"));
}

void UHeroAbility_BaseMeleeAttack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
    GetWorld()->GetTimerManager().ClearTimer(WeaponTraceTimerHandle);
    WeaponTraceTimerHandle.Invalidate();
    ResetHitData();
    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}



void UHeroAbility_BaseMeleeAttack::ToggleWeaponTrace(FGameplayEventData InEventDtata)
{
    if (!CachedWeaponStaticMesh) return;

    EPGToggleType ToggleType = static_cast<EPGToggleType>(static_cast<int32>(InEventDtata.EventMagnitude));

    if (ToggleType == EPGToggleType::On)
    {
        ResetHitData();
        GetWorld()->GetTimerManager().SetTimer(
            WeaponTraceTimerHandle,
            this,
            &UHeroAbility_BaseMeleeAttack::PerformWeaponTrace,
            WeaponTraceInterval,
            true
        );
    }
    else
    {
        ResetHitData();
        GetWorld()->GetTimerManager().ClearTimer(WeaponTraceTimerHandle);
        WeaponTraceTimerHandle.Invalidate();
    }
}

// 트레이스를 수행하여 히튼한 액터들을 찾는 함수
void UHeroAbility_BaseMeleeAttack::PerformWeaponTrace()
{
    // 트레이스를 켜서 액터를 감지하여 overrapedActor에 추가
    FVector StartLocation = CachedWeaponStaticMesh->GetSocketLocation(TEXT("WeaponTraceStart"));
    FVector EndLocation = CachedWeaponStaticMesh->GetSocketLocation(TEXT("WeaponTraceEnd"));
    ETraceTypeQuery TraceChannel = UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel1); //DefaultEngine.ini에서 선언된 AttackTrace 채널
    TArray<FHitResult> OutHits;

    UKismetSystemLibrary::SphereTraceMulti(
        this,
        StartLocation,
        EndLocation,
        WeaponTraceSphereRadius,
        TraceChannel,
        false,
        TArray<AActor*>(),
        bEnableTraceDebug ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None,
        OutHits,
        true,
        FLinearColor::Red, FLinearColor::Green, TraceDebugDuration
    );


    if (OutHits.Num() <= 0)
        return;

    // 히트된 액터들 처리
    for (FHitResult& OutHit : OutHits)
    {
        AActor* HitActor = OutHit.GetActor();
        
        if (UPGFunctionLibrary::IsTargetCharacterIsHostile(GetAvatarActorFromActorInfo(), HitActor))
        {
            if (HitActor && HitActor != GetAvatarActorFromActorInfo())
            {
                HandleApplyDamage(HitActor);
            }
        }
    }
}

// 상대방에게 콜리전이 오버랩될때, 이벤트 트리거에 의해 실행되는 데미지 적용 함수
void UHeroAbility_BaseMeleeAttack::HandleApplyDamage(AActor* InTargetActor)
{   
    //// 이미 히트된 액터는 무시
    if (HitActors.Contains(InTargetActor))
    {
        return;
    }
    
    if(CurrentHitTargets >= MaxHitTargets)
    {
        // 최대 타겟 수에 도달했으므로 트레이스 타이머 종료
        GetWorld()->GetTimerManager().ClearTimer(WeaponTraceTimerHandle);
        WeaponTraceTimerHandle.Invalidate();
        return;
    }

    UE_LOG(LogTemp, Warning, TEXT("Target Actor : %s"), *GetNameSafe(InTargetActor));
    
    //// 게임플레이 큐 실행
    //UGameplayCueFunctionLibrary::ExecuteGameplayCueOnActor(GetAvatarActorFromActorInfo(), MeleeAttackCueTag, FGameplayCueParameters());

    //// TODO : 스킬의 데미지 Multiflier를 변수화
    float SkillMultiplierValue = MeleeAttackSkillMultiplier.GetValueAtLevel(GetAbilityLevel());
    FGameplayEffectSpecHandle EffectSpecHandle = MakeHeroDamageEffectSpecHandle(MeleeAttackDamageEffectClass, SkillMultiplierValue);
    
    NativeApplyEffectSpecHandleToTarget(InTargetActor, EffectSpecHandle);
    CurrentHitTargets++;
    HitActors.Add(InTargetActor);

    // 히트된 액터에게 히트 반응 이벤트 전송
    UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(InTargetActor, PGGameplayTags::Shared_Event_HitReact, FGameplayEventData());
}

void UHeroAbility_BaseMeleeAttack::OnMontageFinished()
{
    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}

void UHeroAbility_BaseMeleeAttack::ResetHitData()
{
    HitActors.Empty();
    CurrentHitTargets = 0;
}
