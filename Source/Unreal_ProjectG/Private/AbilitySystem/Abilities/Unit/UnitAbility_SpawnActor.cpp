// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Unit/UnitAbility_SpawnActor.h"
#include "PGGameplayTags.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"



void UUnitAbility_SpawnActor::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
}

void UUnitAbility_SpawnActor::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{

}

void UUnitAbility_SpawnActor::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
}

void UUnitAbility_SpawnActor::SpawnActorEvent(FGameplayEventData InEventData)
{
}

void UUnitAbility_SpawnActor::OnMontageFinished()
{
}
