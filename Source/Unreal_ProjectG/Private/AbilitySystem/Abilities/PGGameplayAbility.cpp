// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/PGGameplayAbility.h"
#include "AbilitySystem/PGAbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Components/Combat/PawnCombatComponent.h"

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

UPawnCombatComponent* UPGGameplayAbility::GetPawnCombatComponent() const
{
    return GetAvatarActorFromActorInfo()->FindComponentByClass<UPawnCombatComponent>();
}

UPGAbilitySystemComponent* UPGGameplayAbility::GetPGAbilitySystemComponentFromActorInfo() const
{
    return Cast<UPGAbilitySystemComponent>(GetAbilitySystemComponentFromActorInfo());
}

FActiveGameplayEffectHandle UPGGameplayAbility::NativeApplyEffectSpecHandleToTarget(AActor* TargetActor, const FGameplayEffectSpecHandle& InSpecHandle)
{
    // TargetActor의 AbilitySystemComponent 가져오기
    UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);

    checkf(TargetASC, TEXT("TargetActor의 AbilitySystemComponent가 없습니다. TargetActor : %s"), *GetNameSafe(TargetActor));
    checkf(InSpecHandle.IsValid(), TEXT("InSpecHandle가 유효하지 않습니다. TargetActor : %s"), *GetNameSafe(TargetActor));

    return GetPGAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(
        *InSpecHandle.Data, TargetASC);
}



FActiveGameplayEffectHandle UPGGameplayAbility::BP_ApplyEffectSpecHandleToTarget(AActor* TargetActor, const FGameplayEffectSpecHandle& InSpecHandle, EPGSuccessType& OutSuccessType)
{
    FActiveGameplayEffectHandle ActivateGameplayEffectHandle = NativeApplyEffectSpecHandleToTarget(TargetActor, InSpecHandle);

    OutSuccessType = ActivateGameplayEffectHandle.IsValid() ? EPGSuccessType::Successful : EPGSuccessType::Failed;

    return ActivateGameplayEffectHandle;
}

void UPGGameplayAbility::NativeRemoveActiveGameplayEffectFromTarget(AActor* TargetActor, const FActiveGameplayEffectHandle& EffectHandle)
{
    // TargetActor의 AbilitySystemComponent 가져오기
    UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
    
    checkf(TargetASC, TEXT("TargetActor의 AbilitySystemComponent가 없습니다. TargetActor : %s"), *GetNameSafe(TargetActor));
    checkf(EffectHandle.IsValid(), TEXT("EffectHandle이 유효하지 않습니다. TargetActor : %s"), *GetNameSafe(TargetActor));
    
    TargetASC->RemoveActiveGameplayEffect(EffectHandle);
}
