// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/PGUnitGameplayAbility.h"
#include "UnitAbility_BaseMeleeAttack.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_PROJECTG_API UUnitAbility_BaseMeleeAttack : public UPGUnitGameplayAbility
{
	GENERATED_BODY()
	
public:
    UUnitAbility_BaseMeleeAttack();

    virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
    virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:
    UFUNCTION(BlueprintCallable, Category = "Ability|Melee Attack")
    void HandleApplyDamage(FGameplayEventData InEventData);

    UFUNCTION()
    void OnMontageFinished();

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability|Melee Attack")
    TSubclassOf<UGameplayEffect> MeleeAttackDamageEffectClass;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability|Melee Attack")
    FGameplayTag MeleeAttackCueTag;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability|Melee Attack")
    int32 MaxHitTargets = 1;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability|Melee Attack")
    TArray<TObjectPtr<UAnimMontage>> MeleeAttackMontages;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability|Melee Attack")
    FScalableFloat MeleeAttackSkillMultiplier;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability|Melee Attack")
    float MeleeAttackDamageRadius = 100.f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability|Melee Attack")
    int32 MaxHitTargets = 1;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability|Debug")
    bool bEnableTraceDebug = false;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability|Debug")
    float TraceDebugDuration = 0.1f;


    //UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability|Melee Attack")
    //float MeleeAttackRange = 200.f;

private:
    TArray<TWeakObjectPtr<AActor>> CachedTargetActors;
};
