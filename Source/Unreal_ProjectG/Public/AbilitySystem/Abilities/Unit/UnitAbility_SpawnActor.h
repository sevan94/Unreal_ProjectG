// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/PGUnitGameplayAbility.h"
#include "UnitAbility_SpawnActor.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_PROJECTG_API UUnitAbility_SpawnActor : public UPGUnitGameplayAbility
{
	GENERATED_BODY()
public:
    virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
    virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
    virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:
    UFUNCTION(BlueprintCallable, Category = "Ability|Spawn Actor")
    void SpawnActorEvent(FGameplayEventData InEventData);

    UFUNCTION()
    void OnMontageFinished();

protected:
    // 장판(AOE)에 전달할 데미지/디버프 이펙트 클래스
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability|Spawn Actor")
    TSubclassOf<UGameplayEffect> SpawnEffectClass;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability|Spawn Actor")
    TObjectPtr<UAnimMontage> SpawnMontage;

    // 스킬 위력
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability|Spawn Actor")
    FScalableFloat SpawnSkillMultiplier;

    // 스폰할 액터
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability|Spawn Actor")
    TSubclassOf<AActor> SpawnedActorClass;
};
