// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Player/GA_Die.h"
#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_WaitDelay.h"
#include "Character/Hero/HeroCharacter.h"
#include "Character/PGCharacterBase.h"
#include "AbilitySystem/PGAbilitySystemComponent.h"

UGA_Die::UGA_Die()
{
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGA_Die::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
    {
        UE_LOG(LogTemp, Warning, TEXT("GA_Die : Actor UnAvailable"));
        EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
        return;
    }

    AHeroCharacter* Hero = Cast<AHeroCharacter>(ActorInfo->AvatarActor.Get());
    if (Hero)
    {
        UE_LOG(LogTemp, Log, TEXT("GA_Die Activated"));
        Hero->MakeHeroDead();

        UAbilityTask_WaitDelay* DelayTask = UAbilityTask_WaitDelay::WaitDelay(this, RespawnTime);
        if (DelayTask)
        {
            DelayTask->OnFinish.AddDynamic(this, &UGA_Die::RespawnHero);
            DelayTask->ReadyForActivation();
        }
    }
    else
    {
        UE_LOG(LogTemp, Log, TEXT("Hero Unavailable"));
    }
}

void UGA_Die::RespawnHero()
{
    AHeroCharacter* Hero = Cast<AHeroCharacter>(CurrentActorInfo->AvatarActor.Get());
    if (Hero)
    {
        Hero->SpawnHero();
        
        UPGAbilitySystemComponent* HeroAbility = Hero->GetPGAbilitySystemComponent();
        if (FullHealth && HeroAbility)
        {
            FGameplayEffectContextHandle Context = HeroAbility->MakeEffectContext();
            Context.AddSourceObject(this);
            FGameplayEffectSpecHandle Spec = HeroAbility->MakeOutgoingSpec(FullHealth, GetAbilityLevel(), Context);
            HeroAbility->ApplyGameplayEffectSpecToSelf(*Spec.Data.Get());
        }
    }
    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
