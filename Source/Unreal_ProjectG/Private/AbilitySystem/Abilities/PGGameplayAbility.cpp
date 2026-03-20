// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/PGGameplayAbility.h"
#include "AbilitySystem/PGAbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "PGGameplayTags.h"

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

FGameplayEffectSpecHandle UPGGameplayAbility::MakeOutgoingEffectSpec(TSubclassOf<UGameplayEffect> EffectClass)
{
    check(EffectClass);

    // 게임 플레이 이펙트 컨텍스트 생성
    // 게임 플레이 이펙트 컨텍스트는 이펙트가 어디서 왔는지, 누가 적용했는지 등의 정보를 담고 있음
    FGameplayEffectContextHandle ContextHandle = GetPGAbilitySystemComponentFromActorInfo()->MakeEffectContext();
    ContextHandle.SetAbility(this);
    ContextHandle.AddSourceObject(GetAvatarActorFromActorInfo());
    ContextHandle.AddInstigator(GetAvatarActorFromActorInfo(), GetAvatarActorFromActorInfo());

    // 게임 플레이 이펙트 스펙 생성
    // 게임 플레이 이펙트 스펙은 이펙트의 구체적인 속성들을 담고 있음
    FGameplayEffectSpecHandle EffectSpecHandle = GetPGAbilitySystemComponentFromActorInfo()->MakeOutgoingSpec(
        EffectClass,
        GetAbilityLevel(),
        ContextHandle);

    return EffectSpecHandle;
}

FGameplayEffectSpecHandle UPGGameplayAbility::MakeOutgoingEffectSpecWithMultiplier(TSubclassOf<UGameplayEffect> EffectClass, float SkillMultiflier)
{
    check(EffectClass);

    // 게임 플레이 이펙트 스펙 생성
    // 게임 플레이 이펙트 스펙은 이펙트의 구체적인 속성들을 담고 있음
    FGameplayEffectSpecHandle EffectSpecHandle = MakeOutgoingEffectSpec(EffectClass);

    // 이펙트 스펙에 SetByCaller 매개변수 설정하여 스킬 배율값을 전달
    EffectSpecHandle.Data->SetSetByCallerMagnitude(
        PGGameplayTags::Shared_SetByCaller_SkillMultiplier,
        SkillMultiflier
    );

    return EffectSpecHandle;
}

FGameplayEffectSpecHandle UPGGameplayAbility::MakeOutgoingEffectSpecFromEffectConfig(const FEffectConfig& EffectConfig)
{
    checkf(EffectConfig.EffectClass, TEXT("EffectConfig의 EffectClass가 유효하지 않습니다."));
    
    // 게임 플레이 이펙트 스펙 생성
    FGameplayEffectSpecHandle EffectSpecHandle = MakeOutgoingEffectSpec(EffectConfig.EffectClass);

    // EffectConfig에서 설정된 값들을 스펙에 적용
    if (!FMath::IsNearlyZero(EffectConfig.Multiplier))
    {
        EffectSpecHandle.Data->SetSetByCallerMagnitude(
            PGGameplayTags::Shared_SetByCaller_SkillMultiplier,
            EffectConfig.Multiplier
        );
    }

    if (EffectConfig.Duration > 0.f)
    {
        EffectSpecHandle.Data->SetSetByCallerMagnitude(
            PGGameplayTags::Shared_SetByCaller_Duration,
            EffectConfig.Duration
        );
    }

    if(EffectConfig.BaseAmount > 0.f)
    {
        EffectSpecHandle.Data->SetSetByCallerMagnitude(
            PGGameplayTags::Shared_SetByCaller_BaseAmount,
            EffectConfig.BaseAmount
        );
    }

    return EffectSpecHandle;
}

TArray<FGameplayEffectSpecHandle> UPGGameplayAbility::MakeOutgoingEffectSpecsFromEffectConfigs(const TArray<FEffectConfig>& EffectConfigs)
{
    TArray<FGameplayEffectSpecHandle> EffectSpecHandles;
    
    for (const FEffectConfig& EffectConfig : EffectConfigs)
    {
        FGameplayEffectSpecHandle EffectSpecHandle = MakeOutgoingEffectSpecFromEffectConfig(EffectConfig);
        if (EffectSpecHandle.IsValid())
        {
            EffectSpecHandles.Add(EffectSpecHandle);
        }
    }
    return EffectSpecHandles;
}

