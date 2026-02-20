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

    virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:
    UFUNCTION(BlueprintCallable, Category = "Ability|AOE Attack")
    void OnHitLocationReady(FVector InHitLocation);

    UFUNCTION()
    void OnApplyAOEDamage(FGameplayEventData EventData);

    UFUNCTION()
    void OnAOEMontageFinished();
protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability|AOE Attack")
    TSubclassOf<UGameplayEffect> AOEAttackDamageEffectClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability|AOE Attack")
    TObjectPtr<UAnimMontage> AOEAttackMontage;

    // 데미지가 들어갈 때 호출될 게임 플레이 큐
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability|AOE Attack")
    FGameplayTag AOEImpactCueTag;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability|AOE Attack")
    int32 MaxHitTargets = 3;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability|AOE Attack")
    float AOEAttackRadius = 300.f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability|AOE Attack")
    FScalableFloat AOEAttackSkillMultiplier;

private:
    FVector CachedHitLocation;
    TArray<AActor*> HitActors;
};
