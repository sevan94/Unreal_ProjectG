// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/PGHeroGameplayAbility.h"
#include "HeroAbility_BaseProjectileAttack.generated.h"

class APGProjectileBase;
/**
 * 
 */
UCLASS()
class UNREAL_PROJECTG_API UHeroAbility_BaseProjectileAttack : public UPGHeroGameplayAbility
{
	GENERATED_BODY()

public:
    UHeroAbility_BaseProjectileAttack();

    virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
    virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
    virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:
    // 프로젝타일 스폰 함수
    UFUNCTION(BlueprintCallable, Category = "Ability|Projectile Attack")
    void SpawnProjectile(FGameplayEventData InEventData);

    UFUNCTION()
    void OnMontageFinished();

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability|Projectile Attack")
    TSubclassOf<UGameplayEffect> ProjectileAttackDamageEffectClass;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability|Projectile Attack")
    TObjectPtr<UAnimMontage> ProjectileAttackMontage;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability|Projectile Attack")
    FScalableFloat ProjectileAttackSkillMultiplier;

    // 프로젝타일 클래스
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability|Projectile Attack")
    TSubclassOf<APGProjectileBase> SpawnedProjectileClass;

private:
    TObjectPtr<UStaticMeshComponent> CachedWeaponStaticMesh;
};
