// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/PGGameplayAbility.h"
#include "SharedAbility_BuffAura.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_PROJECTG_API USharedAbility_BuffAura : public UPGGameplayAbility
{
	GENERATED_BODY()
	
public:
    virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
    virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;


protected:
    UFUNCTION(BlueprintCallable, Category = "Ability|Buff Aura")
    void ApplyBuffAuraEffectToTarget(AActor* TargetActor);

private:
    UFUNCTION()
    void OnAuraBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
    UFUNCTION()
    void OnAuraEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability|Buff Aura")
    TSubclassOf<UGameplayEffect> BuffAuraEffectClass;

    UPROPERTY(EditDefaulTSoNLY, BlueprintReadOnly, Category = "Ability|Buff Aura")
    float BuffAuraRadius;

private:
    UPROPERTY()
    TObjectPtr<class USphereComponent> BuffAuraSphere;

    UPROPERTY()
    TMap<AActor*, FActiveGameplayEffectHandle> ActiveBuffsOnTargets;
};
