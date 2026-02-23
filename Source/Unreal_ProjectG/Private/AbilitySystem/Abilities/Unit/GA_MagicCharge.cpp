// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Unit/GA_MagicCharge.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "GameFramework/Character.h"

UGA_MagicCharge::UGA_MagicCharge()
{
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGA_MagicCharge::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    UE_LOG(LogTemp, Warning, TEXT("===== MagicCharge ActivateAbility 진입 성공! ====="));
    if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
    {
        UE_LOG(LogTemp, Error, TEXT("CommitAbility 실패! (비용이나 쿨타임 문제)"));
        EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
        return;
    }

    ACharacter* Character = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
    if (!Character)
    {
        EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
        return;
    }

    if (ChargeEffect)
    {
        SpawnedEffect = UNiagaraFunctionLibrary::SpawnSystemAttached(
            ChargeEffect,
            Character->GetMesh(),
            MuzzleSocketName,
            FVector::ZeroVector,
            FRotator::ZeroRotator,
            EAttachLocation::SnapToTarget,
            true
        );
        UE_LOG(LogTemp, Log, TEXT("이펙트스폰"));

    }

    if (ChargeMontage)
    {
        UAbilityTask_PlayMontageAndWait* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
            this,
            TEXT("ChargeMontageTask"),
            ChargeMontage
        );

        MontageTask->OnCompleted.AddDynamic(this, &UGA_MagicCharge::OnMontageCompleted);
        MontageTask->OnBlendOut.AddDynamic(this, &UGA_MagicCharge::OnMontageCompleted);

        MontageTask->OnInterrupted.AddDynamic(this, &UGA_MagicCharge::OnMontageCancelled);
        MontageTask->OnCancelled.AddDynamic(this, &UGA_MagicCharge::OnMontageCancelled);

        MontageTask->ReadyForActivation();
        UE_LOG(LogTemp, Log, TEXT("차지몽타주"));

    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("ChargeMontage가 비어있습니다! 블루프린트 세팅을 확인하세요."));
        EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
    }
}

void UGA_MagicCharge::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
    if (IsValid(SpawnedEffect))
    {
        SpawnedEffect->DestroyComponent();
        SpawnedEffect = nullptr;
    }

    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGA_MagicCharge::OnMontageCompleted()
{
    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UGA_MagicCharge::OnMontageCancelled()
{
    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}
