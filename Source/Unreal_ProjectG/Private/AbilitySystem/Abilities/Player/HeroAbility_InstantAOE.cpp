// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Player/HeroAbility_InstantAOE.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "PGGameplayTags.h"

UHeroAbility_InstantAOE::UHeroAbility_InstantAOE()
{
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UHeroAbility_InstantAOE::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    if(!CommitAbility(Handle, ActorInfo, ActivationInfo))
    {
        EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
        return;
    }

    UAnimMontage* MontageToPlay = AOEConfig.Montage.Get();

    if (!MontageToPlay)
    {
        SpawnAndInitializeAOEActor(GetInstantAOESpawnLocation(), GetInstantAOESpawnRotation());
        EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
        return;
    }

    // 애니메이션 몽타주 재생
    UAbilityTask_PlayMontageAndWait* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, MontageToPlay);
    
    if (MontageTask)
    {
        MontageTask->OnCompleted.AddUniqueDynamic(this, &UHeroAbility_InstantAOE::OnMontageFinished);
        MontageTask->OnInterrupted.AddUniqueDynamic(this, &UHeroAbility_InstantAOE::OnMontageFinished);
        MontageTask->OnBlendOut.AddUniqueDynamic(this, &UHeroAbility_InstantAOE::OnMontageFinished);
        MontageTask->OnCancelled.AddUniqueDynamic(this, &UHeroAbility_InstantAOE::OnMontageFinished);
        MontageTask->ReadyForActivation();
    }

    // 게임플레이 이벤트 대기 태스크 생성
    UAbilityTask_WaitGameplayEvent* ExecuteEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, PGGameplayTags::Shared_Event_AOEExecute);
    ExecuteEventTask->EventReceived.AddUniqueDynamic(this, &UHeroAbility_InstantAOE::OnAOEExecuteEventReceived);
    ExecuteEventTask->ReadyForActivation();
}

void UHeroAbility_InstantAOE::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

FVector UHeroAbility_InstantAOE::GetInstantAOESpawnLocation_Implementation() const
{
    const AActor* AvatarActor = GetAvatarActorFromActorInfo();
    return AvatarActor ? AvatarActor->GetActorLocation() : FVector::ZeroVector;
}

FRotator UHeroAbility_InstantAOE::GetInstantAOESpawnRotation_Implementation() const
{
    const AActor* AvatarActor = GetAvatarActorFromActorInfo();
    return AvatarActor ? AvatarActor->GetActorRotation() : FRotator::ZeroRotator;
}

void UHeroAbility_InstantAOE::OnAOEExecuteEventReceived(FGameplayEventData EventData)
{
    SpawnAndInitializeAOEActor(GetInstantAOESpawnLocation(), GetInstantAOESpawnRotation());
}

void UHeroAbility_InstantAOE::OnMontageFinished()
{
    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}