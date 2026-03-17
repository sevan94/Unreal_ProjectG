// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/PGUnitGameplayAbility.h"
#include "UnitAbility_Supporter.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_PROJECTG_API UUnitAbility_Supporter : public UPGUnitGameplayAbility
{
	GENERATED_BODY()
public:
    UUnitAbility_Supporter();

    // DataAsset(Config)을 사용하면 OnGiveAbility에서 변수들을 초기화 가능
    virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
    virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
    virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:
    // 애니메이션 노티파이에서 호출될 실제 장판 생성 및 힐/버프 적용 함수
    UFUNCTION(BlueprintCallable, Category = "Ability|Support")
    void HandleSupportEffect(FGameplayEventData InEventData);

    UFUNCTION()
    void OnMontageFinished();

protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability|Support")

    FUnitBuffAuraAbilityConfig UnitBuffConfig;
};
