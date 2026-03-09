// Fill out your copyright notice in the Description page of Project Settings.
#include "AbilitySystem/Abilities/Unit/UnitAbility_HitReact.h"
#include "TimerManager.h"
#include "Interfaces/VisualEffectTargetInterface.h"

void UUnitAbility_HitReact::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    if(AActor* AvatarActor = ActorInfo->AvatarActor.Get())
    {
        if(IVisualEffectTargetInterface* VisualEffectTarget = Cast<IVisualEffectTargetInterface>(AvatarActor))
        {
            VisualEffectTarget->Execute_SetHitReactEnabled(AvatarActor, true);
        }
    }

    FTimerHandle TimerHandle;
    GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
        {
            EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
        },
        HitFXDuration, false);
}

void UUnitAbility_HitReact::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
    if(AActor* AvatarActor = ActorInfo->AvatarActor.Get())
    {
        if(IVisualEffectTargetInterface* VisualEffectTarget = Cast<IVisualEffectTargetInterface>(AvatarActor))
        {
            VisualEffectTarget->Execute_SetHitReactEnabled(AvatarActor, false);
        }
    }

    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}