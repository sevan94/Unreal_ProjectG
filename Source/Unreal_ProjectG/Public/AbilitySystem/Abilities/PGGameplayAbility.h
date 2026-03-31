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

    // EffectConfig 배열을 기반으로 게임플레이 이펙트 스펙핸들 배열 생성
    TArray<FGameplayEffectSpecHandle> MakeOutgoingEffectSpecsFromEffectConfigs(const TArray<FEffectConfig>& EffectConfigs);

    // EffectConfig을 기반으로 게임플레이 이펙트 스펙핸들 생성
    UFUNCTION(BlueprintPure, Category = "PG|Ability")
    FGameplayEffectSpecHandle MakeOutgoingEffectSpecFromEffectConfig(const FEffectConfig& EffectConfig);

    // 타깃 액터에게 게임플레이 이펙트를 적용하는 네이티브 함수
    FActiveGameplayEffectHandle NativeApplyEffectSpecHandleToTarget(AActor* TargetActor, const FGameplayEffectSpecHandle& InSpecHandle);

protected:
    virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
    virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

    // ActorInfo에서 PGAbilitySystemComponent을 가져오는 헬퍼 함수
    UFUNCTION(BlueprintPure, Category = "PGGameplayAbility")
    UPGAbilitySystemComponent* GetPGAbilitySystemComponentFromActorInfo() const;

    UFUNCTION(BlueprintCallable, Category = "PG|Ability", meta = (DisplayName = "Apply Gameplay Effect Spec Handle To Target Actor", ExpandEnumAsExecs = "OutSuccessType"))
    FActiveGameplayEffectHandle BP_ApplyEffectSpecHandleToTarget(AActor* TargetActor, const FGameplayEffectSpecHandle& InSpecHandle, EPGSuccessType& OutSuccessType);

    // 타깃 액터에게 게임플레이 이펙트를 제거하는 네이티브 함수
    void NativeRemoveActiveEffectFromTarget(AActor* TargetActor, const FActiveGameplayEffectHandle& EffectHandle);

    // 게임 플레이 이펙트 스펙핸들을 생성하는 함수
    UFUNCTION(BlueprintPure, Category = "PG|Ability")
    FGameplayEffectSpecHandle MakeOutgoingEffectSpec(TSubclassOf<UGameplayEffect> EffectClass);

    // 스킬 배율이 적용된 게임플레이 이펙트 스펙핸들 생성
    UFUNCTION(BlueprintPure, Category = "PG|Ability")
    FGameplayEffectSpecHandle MakeOutgoingEffectSpecWithMultiplier(TSubclassOf<UGameplayEffect> EffectClass, float SkillMultiflier);

public:
    // 일단은 UI 사용 용도로 Public하게 두었음, 추후 로직에 들어가면 위치 변경 가능
    // FAbilityEntry 안에 둔것이 아니기 때문에, 스킬 발동 방식은 Upgrade와 기본 스킬 모두 공통되어야 함. 추후, 생각 바뀌면 이동 가능
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (DisplayName = "스킬 발동 방식"))
    ESkillActivationType ActivationType = ESkillActivationType::Instant;

protected:
    UPROPERTY(EditDefaultsOnly, Category = "PGGameplayAbility")
    EPGAbilityActivationPolicy AbilityActivationPolicy = EPGAbilityActivationPolicy::OnTriggered;
};
