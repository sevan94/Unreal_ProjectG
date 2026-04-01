// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/PGGameplayAbility.h"
#include "AbilitySystem/PGAbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "PGGameplayTags.h"
#include "PGFunctionLibrary.h"


UPGGameplayAbility::UPGGameplayAbility()
{
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UPGGameplayAbility::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
    Super::OnGiveAbility(ActorInfo, Spec);

    // 발동 정책에 따라 Passive 능력 자동 활성화
    if(AbilityActivationPolicy == EPGAbilityActivationPolicy::OnGiven)
    {
        if (ActorInfo && !Spec.IsActive())
        {
            ActorInfo->AbilitySystemComponent->TryActivateAbility(Spec.Handle);
        }
    }
}

void UPGGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

    // 발동 정책에 따라 Passive 능력 자동 정리
    if (AbilityActivationPolicy == EPGAbilityActivationPolicy::OnGiven)
    {
        if (ActorInfo)
        {
            ActorInfo->AbilitySystemComponent->ClearAbility(Handle);
        }
    }
}


// ====================================================================================================
// 랩핑 함수들 
// ====================================================================================================

UPGAbilitySystemComponent* UPGGameplayAbility::GetPGAbilitySystemComponentFromActorInfo() const
{
    return Cast<UPGAbilitySystemComponent>(GetAbilitySystemComponentFromActorInfo());
}

FActiveGameplayEffectHandle UPGGameplayAbility::NativeApplyEffectSpecHandleToTarget(AActor* TargetActor, const FGameplayEffectSpecHandle& InSpecHandle)
{
    checkf(InSpecHandle.IsValid(), TEXT("InSpecHandle가 유효하지 않습니다. TargetActor : %s"), *GetNameSafe(TargetActor));

    return UPGFunctionLibrary::ApplyGameplayEffectSpecHandleToTargetActor(GetAvatarActorFromActorInfo(), TargetActor, InSpecHandle);
}

FActiveGameplayEffectHandle UPGGameplayAbility::BP_ApplyEffectSpecHandleToTarget(AActor* TargetActor, const FGameplayEffectSpecHandle& InSpecHandle, EPGSuccessType& OutSuccessType)
{
    FActiveGameplayEffectHandle ActivateGameplayEffectHandle = NativeApplyEffectSpecHandleToTarget(TargetActor, InSpecHandle);

    OutSuccessType = ActivateGameplayEffectHandle.IsValid() ? EPGSuccessType::Successful : EPGSuccessType::Failed;

    return ActivateGameplayEffectHandle;
}

void UPGGameplayAbility::NativeRemoveActiveEffectFromTarget(AActor* TargetActor, const FActiveGameplayEffectHandle& EffectHandle)
{
    checkf(EffectHandle.IsValid(), TEXT("EffectHandle이 유효하지 않습니다. TargetActor : %s"), *GetNameSafe(TargetActor));
    UPGFunctionLibrary::NativeRemoveActiveGameplayEffectFromTarget(TargetActor, EffectHandle);
}

FGameplayEffectSpecHandle UPGGameplayAbility::MakeOutgoingEffectSpec(TSubclassOf<UGameplayEffect> EffectClass)
{
    check(EffectClass);

    return UPGFunctionLibrary::MakeOutgoingGameplayEffectSpec(GetPGAbilitySystemComponentFromActorInfo(), EffectClass, GetAvatarActorFromActorInfo(), this, GetAbilityLevel());
}

FGameplayEffectSpecHandle UPGGameplayAbility::MakeOutgoingEffectSpecWithMultiplier(TSubclassOf<UGameplayEffect> EffectClass, float SkillMultiflier)
{
    check(EffectClass);

    return UPGFunctionLibrary::MakeOutgoingGameplayEffectSpecWithMultiplier(GetPGAbilitySystemComponentFromActorInfo(), EffectClass, SkillMultiflier, GetAvatarActorFromActorInfo(), this, GetAbilityLevel());
}

FGameplayEffectSpecHandle UPGGameplayAbility::MakeOutgoingEffectSpecFromEffectConfig(const FEffectConfig& EffectConfig)
{
    checkf(EffectConfig.EffectClass, TEXT("EffectConfig의 EffectClass가 유효하지 않습니다."));

    return UPGFunctionLibrary::MakeOutgoingGameplayEffectSpecFromEffectConfig(GetPGAbilitySystemComponentFromActorInfo(), EffectConfig, GetAvatarActorFromActorInfo(), this, GetAbilityLevel());
}

TArray<FGameplayEffectSpecHandle> UPGGameplayAbility::MakeOutgoingEffectSpecsFromEffectConfigs(const TArray<FEffectConfig>& EffectConfigs)
{
    return UPGFunctionLibrary::MakeOutgoingGameplayEffectSpecsFromEffectConfigs(GetPGAbilitySystemComponentFromActorInfo(), EffectConfigs, GetAvatarActorFromActorInfo(), this, GetAbilityLevel());
}

