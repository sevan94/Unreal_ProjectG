// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Hero/PGHeroSkillGameplayAbility.h"
#include "PGGameplayTags.h"

UPGHeroSkillGameplayAbility::UPGHeroSkillGameplayAbility()
{
    ActivationBlockedTags.AddTag(PGGameplayTags::Player_Ability_Die);
}

void UPGHeroSkillGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    HeroSkillData = Cast<UDataAsset_HeroSkillData>(GetCurrentSourceObject());

    if (!HeroSkillData || HeroSkillData->ActionSequence.IsEmpty());
    {
        
    }
}

void UPGHeroSkillGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{

}

void UPGHeroSkillGameplayAbility::ExecuteNextAction()
{

}

void UPGHeroSkillGameplayAbility::OnActionCompleted(FGameplayAbilityTargetDataHandle TargetData)
{

}

void UPGHeroSkillGameplayAbility::OnActionCancelled(FGameplayAbilityTargetDataHandle TargetData)
{

}
