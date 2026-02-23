// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/PGUnitGameplayAbility.h"
#include "GA_MagicCharge.generated.h"
class UNiagaraSystem;
class UNiagaraComponent;
/**
 * 
 */
UCLASS()
class UNREAL_PROJECTG_API UGA_MagicCharge : public UPGUnitGameplayAbility
{
	GENERATED_BODY()
public:
    UGA_MagicCharge();

protected:
    virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

    virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Magic")
    UAnimMontage* ChargeMontage;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Magic")
    UNiagaraSystem* ChargeEffect;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Magic")
    FName MuzzleSocketName = TEXT("Muzzle");

private:
    // 생성된 나이아가라 이펙트를 추적하기 위한 포인터
    UPROPERTY()
    UNiagaraComponent* SpawnedEffect;

    // 몽타주가 끝나거나 취소되었을 때 실행할 콜백 함수들
    UFUNCTION()
    void OnMontageCompleted();

    UFUNCTION()
    void OnMontageCancelled();
};
