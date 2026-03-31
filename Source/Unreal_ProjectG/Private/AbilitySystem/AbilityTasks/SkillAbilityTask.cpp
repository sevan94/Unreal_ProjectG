// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AbilityTasks/SkillAbilityTask.h"
#include "GameplayCueFunctionLibrary.h"

FGameplayEffectSpecHandle USkillAbilityTask::AddActorCueIntoSpecHandle(TArray<FGameplayEffectSpecHandle>& SpecHandles, EEffectType InEffectType, const FGameplayTag& InActorCueTag)
{
    return FGameplayEffectSpecHandle();
}

void USkillAbilityTask::ExecuteSstaticCue(AActor* TargetActor, const FGameplayTag& InStaticCueTag, EEffectType InEffectType, const FGameplayEffectContextHandle& EffectContext) const
{

}

void USkillAbilityTask::EmitRuntimeEvent(const FGameplayTag& EventTag, const FGameplayAbilityTargetDataHandle& TargetData)
{
    if (EventTag.IsValid())
    {
        OnRuntimeEvent.Broadcast(EventTag, TargetData);
    }
}
