// Fill out your copyright notice in the Description page of Project Settings.

#include "GA_InitializeHero.h"
#include "Character/Hero/HeroCharacter.h"
#include "AbilitySystem/PGAbilitySystemComponent.h"

UGA_InitializeHero::UGA_InitializeHero()
{
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGA_InitializeHero::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
    {
        EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
        return;
    }

    AHeroCharacter* Hero = Cast<AHeroCharacter>(ActorInfo->AvatarActor.Get());

    if (Hero && FullHeal && FillCost && StartCost)
    {
        UPGAbilitySystemComponent* HeroAbility = Hero->GetPGAbilitySystemComponent();
        FGameplayEffectContextHandle Context = HeroAbility->MakeEffectContext();
        Context.AddSourceObject(this);
        FGameplayEffectSpecHandle CostSpec = HeroAbility->MakeOutgoingSpec(FillCost, GetAbilityLevel(), Context);
        FGameplayEffectSpecHandle HealSpec = HeroAbility->MakeOutgoingSpec(FullHeal, GetAbilityLevel(), Context);
        FGameplayEffectSpecHandle StartCostSpec = HeroAbility->MakeOutgoingSpec(StartCost, GetAbilityLevel(), Context);
        HeroAbility->ApplyGameplayEffectSpecToSelf(*CostSpec.Data.Get());
        HeroAbility->ApplyGameplayEffectSpecToSelf(*HealSpec.Data.Get());
        HeroAbility->ApplyGameplayEffectSpecToSelf(*StartCostSpec.Data.Get());
    }
}
