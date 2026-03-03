// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/PGUnitGameplayAbility.h"
#include "UnitAbility_HitReact.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_PROJECTG_API UUnitAbility_HitReact : public UPGUnitGameplayAbility
{
	GENERATED_BODY()
	
public:
    virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
    virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HitReact")
    float HitFXDuration = 0.2f;

private:
    UPROPERTY()
    TWeakObjectPtr<UMeshComponent> OwnerMesh;
};
