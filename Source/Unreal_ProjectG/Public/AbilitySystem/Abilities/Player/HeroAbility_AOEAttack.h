// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/PGHeroGameplayAbility.h"
#include "HeroAbility_AOEAttack.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_PROJECTG_API UHeroAbility_AOEAttack : public UPGHeroGameplayAbility
{
	GENERATED_BODY()
	
public:
    UHeroAbility_AOEAttack();

    virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;

    //virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
    virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:
    UFUNCTION(BlueprintCallable, Category = "Ability|AOE Attack")
    void OnHitLocationReady(FVector InHitLocation);

    UFUNCTION()
    void OnApplyAOEDamage(FGameplayEventData EventData);

    UFUNCTION()
    void OnAOEMontageFinished();
protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability|AOE Attack")
    FHeroCastingAOEAbilityConfig AOEAttackConfig;

private:
    FVector CachedHitLocation;
    TArray<AActor*> HitActors;
};
