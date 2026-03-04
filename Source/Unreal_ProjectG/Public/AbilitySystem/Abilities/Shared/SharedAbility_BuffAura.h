// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/PGGameplayAbility.h"
#include "Types/PGGasTypes.h"
#include "SharedAbility_BuffAura.generated.h"



/**
 * 
 */
UCLASS()
class UNREAL_PROJECTG_API USharedAbility_BuffAura : public UPGGameplayAbility
{
	GENERATED_BODY()
	
public:
    virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
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

private:
    void BuildCachedBuffEffectSpecs();

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability|Buff Aura")
    TArray<TSubclassOf<UGameplayEffect>> BuffAuraEffectClasses;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability|Buff Aura")
    float BuffAuraRadius;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability|Buff Aura")
    TObjectPtr<UMaterialInterface> AuraRadiusDecalMaterial;

private:
    UPROPERTY()
    TObjectPtr<class USphereComponent> BuffAuraSphere;

    UPROPERTY()
    TObjectPtr<class UDecalComponent> BuffAuraDecal;

    UPROPERTY()
    TMap<AActor*, FActiveGameplayEffectHandle> ActiveBuffsOnTargets;

    // 적용할 이펙트 스펙핸들 캐싱
    TArray<FGameplayEffectSpecHandle> CachedBuffEffectSpecs;
};
