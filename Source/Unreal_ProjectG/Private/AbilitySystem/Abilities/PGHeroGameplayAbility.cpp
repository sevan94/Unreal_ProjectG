// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/PGHeroGameplayAbility.h"
#include "AbilitySystem/PGAbilitySystemComponent.h"
#include "Character/Hero/HeroCharacter.h"
#include "Character/HeroController.h"
#include "PGGameplayTags.h"

UPGHeroGameplayAbility::UPGHeroGameplayAbility()
{
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

AHeroCharacter* UPGHeroGameplayAbility::GetHeroCharacterFromActorInfo()
{
    if (!CachedHeroCharacter.IsValid())
    {
        CachedHeroCharacter = Cast<AHeroCharacter>(CurrentActorInfo->AvatarActor);
    }

    return CachedHeroCharacter.IsValid() ? CachedHeroCharacter.Get() : nullptr;
}


AHeroController* UPGHeroGameplayAbility::GetHeroControllerFromActorInfo()
{
    if (CachedHeroController.IsValid())
    {
        CachedHeroController = Cast<AHeroController>(CurrentActorInfo->PlayerController);
    }
    return CachedHeroController.IsValid() ? CachedHeroController.Get() : nullptr;
}

FGameplayEffectSpecHandle UPGHeroGameplayAbility::MakeHeroDamageEffectSpecHandle(TSubclassOf<UGameplayEffect> EffectClass, float SkillMultiflier)
{
    check(EffectClass);

    FGameplayEffectContextHandle ContextHandle = GetPGAbilitySystemComponentFromActorInfo()->MakeEffectContext();
    ContextHandle.SetAbility(this);
    ContextHandle.AddSourceObject(GetAvatarActorFromActorInfo());
    ContextHandle.AddInstigator(GetAvatarActorFromActorInfo(), GetAvatarActorFromActorInfo());

    FGameplayEffectSpecHandle EffectSpecHandle = GetPGAbilitySystemComponentFromActorInfo()->MakeOutgoingSpec(
        EffectClass,
        GetAbilityLevel(),
        ContextHandle
    );

    // SetByCaller 매개변수로 Map(태그, 값) 형태로 스킬 데미지 배율 전달
    EffectSpecHandle.Data->SetSetByCallerMagnitude(
        PGGameplayTags::Shared_SetByCaller_SkillMultiplier,
        SkillMultiflier
       );

    // 추가적으로 넘길 속성들이 있다면 여기에 추가

    return EffectSpecHandle;
}
