// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/PGHeroGameplayAbility.h"
#include "HeroAbility_BaseMeleeAttack.generated.h"


class UGameplayEffect;
/**
 * 
 */
UCLASS()
class UNREAL_PROJECTG_API UHeroAbility_BaseMeleeAttack : public UPGHeroGameplayAbility
{
	GENERATED_BODY()
	
public:
    virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
    virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
    virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:
    // 트레이스 타이머 토글
    UFUNCTION(BlueprintCallable, Category = "Ability|Melee Attack")
    void ToggleWeaponTrace(FGameplayEventData InEventData);

    // 데미지 적용 핸들러
    UFUNCTION(BlueprintCallable, Category = "Ability|Melee Attack")
    void HandleApplyDamage(AActor* InTargetActor);

    UFUNCTION(BlueprintCallable, Category = "Ability|Melee Attack")
    void PerformWeaponTrace();

    UFUNCTION()
    void OnMontageFinished();

private:
    void ResetHitData();
protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability|Melee Attack")
    FHeroMeleeAttackAbilityConfig MeleeAttackConfig;


    // ========================================================================
    // 디버그 
    UPROPERTY(EditDefaultsOnly, Category = "Ability|Melee Attack")
    float WeaponTraceInterval = 0.1f;

    UPROPERTY(EditDefaultsOnly, Category = "Ability|Debug")
    bool bEnableTraceDebug = false;
    
    UPROPERTY(EditDefaultsOnly, Category = "Ability|Debug")
    float TraceDebugDuration = 1.f;
    // ========================================================================
private:
    TArray<AActor*> HitActors;

    // 무기 스태틱 메시 캐싱
    TObjectPtr<UStaticMeshComponent> CachedWeaponStaticMesh;
    
    // 타격 가능한 현재 적의 수
    int32 CurrentHitTargets = 0;

    FTimerHandle WeaponTraceTimerHandle;
};
