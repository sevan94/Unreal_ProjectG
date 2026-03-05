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
    virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
    virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
    virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:
    UFUNCTION(BlueprintCallable, Category = "Ability|Melee Attack")
    void HandleApplyDamage(FGameplayEventData InEventData);

    UFUNCTION()
    void OnMontageFinished();

protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability|Melee Attack")
    FUnitBaseMeleeAttackAbilityConfig MeleeAttackConfig;


    // 디버그용 트레이스 시각화 토글 및 지속 시간
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability|Debug")
    bool bEnableTraceDebug = false;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability|Debug")
    float TraceDebugDuration = 0.1f;

    //UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability|Melee Attack")
    //float MeleeAttackRange = 200.f;
private:
    TArray<TWeakObjectPtr<AActor>> CachedTargetActors;
};
