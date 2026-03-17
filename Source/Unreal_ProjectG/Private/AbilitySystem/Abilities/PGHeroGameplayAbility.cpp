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