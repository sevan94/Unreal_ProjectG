// Fill out your copyright notice in the Description page of Project Settings.
#include "AbilitySystem/Abilities/Unit/UnitAbility_HitReact.h"
#include "Character/Unit/UnitCharacter.h"
#include "TimerManager.h"

void UUnitAbility_HitReact::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    OwnerMesh = GetUnitCharacterFromActorInfo()->GetMesh();
    if (OwnerMesh.IsValid())
    {
        if (UMaterialInstanceDynamic* DynamicMaterial = OwnerMesh->CreateAndSetMaterialInstanceDynamic(0))
        {
            DynamicMaterial->SetScalarParameterValue(FName("HitFXSwitch"), 1.f);
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
    if (OwnerMesh.IsValid())
    {
        if (UMaterialInstanceDynamic* DynamicMaterial = OwnerMesh->CreateAndSetMaterialInstanceDynamic(0))
        {
            DynamicMaterial->SetScalarParameterValue(FName("HitFXSwitch"), 0.f);
        }
    }
    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}