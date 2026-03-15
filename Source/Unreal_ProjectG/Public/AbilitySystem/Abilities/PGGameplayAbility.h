// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Types/PGEnumTypes.h"
#include "Types/AbilityConfig.h"
#include "PGGameplayAbility.generated.h"

class UPGAbilitySystemComponent;
class UPawnCombatComponent;

UENUM(BlueprintType)
enum class EPGAbilityActivationPolicy : uint8
{
    OnGiven    UMETA(DisplayName = "OnGiven"),       // Give 즉시 활성화 
    OnTriggered UMETA(DisplayName = "OnTriggered"),  // 특정 트리거에 의해 활성화
};

/**
 *  헬퍼 함수 및 공통 기능을 포함하는 기본 게임플레이 능력 클래스
 */
UCLASS()
class UNREAL_PROJECTG_API UPGGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
    UPGGameplayAbility();

protected:
    virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
    virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

    // ActorInfo에서 PGAbilitySystemComponent을 가져오는 헬퍼 함수
    UFUNCTION(BlueprintPure, Category = "PGGameplayAbility")
    UPGAbilitySystemComponent* GetPGAbilitySystemComponentFromActorInfo() const;

    // 타깃 액터에게 게임플레이 이펙트를 적용하는 네이티브 함수
    FActiveGameplayEffectHandle NativeApplyEffectSpecHandleToTarget(AActor* TargetActor, const FGameplayEffectSpecHandle& InSpecHandle);

    UFUNCTION(BlueprintCallable, Category = "PG|Ability", meta = (DisplayName = "Apply Gameplay Effect Spec Handle To Target Actor", ExpandEnumAsExecs = "OutSuccessType"))
    FActiveGameplayEffectHandle BP_ApplyEffectSpecHandleToTarget(AActor* TargetActor, const FGameplayEffectSpecHandle& InSpecHandle, EPGSuccessType& OutSuccessType);

    // 타깃 액터에게 게임플레이 이펙트를 제거하는 네이티브 함수
    void NativeRemoveActiveGameplayEffectFromTarget(AActor* TargetActor, const FActiveGameplayEffectHandle& EffectHandle);

protected:
    UPROPERTY(EditDefaultsOnly, Category = "PGGameplayAbility")
    EPGAbilityActivationPolicy AbilityActivationPolicy = EPGAbilityActivationPolicy::OnTriggered;
};
