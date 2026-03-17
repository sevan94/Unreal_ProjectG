// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/PGUnitGameplayAbility.h"
#include "AbilitySystem/PGAbilitySystemComponent.h"
#include "Character/Unit/UnitCharacter.h"
#include "PGGameplayTags.h"

AUnitCharacter* UPGUnitGameplayAbility::GetUnitCharacterFromActorInfo()
{
    if (!CachedUnitCharacter.IsValid())
    {
        CachedUnitCharacter = Cast<AUnitCharacter>(CurrentActorInfo->AvatarActor);
    }
    return CachedUnitCharacter.IsValid() ? CachedUnitCharacter.Get() : nullptr;
}

FGameplayEffectSpecHandle UPGUnitGameplayAbility::MakeOutgoingEffectSpecToTarget(TSubclassOf<UGameplayEffect> EffectClass, float SkillMultiflier)
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

    // 이펙트 스펙에 SetByCaller 매개변수 설정하여 스킬 배율값을 전달
    EffectSpecHandle.Data->SetSetByCallerMagnitude(
        PGGameplayTags::Shared_SetByCaller_SkillMultiplier,
        SkillMultiflier
    );

    return EffectSpecHandle;
}
