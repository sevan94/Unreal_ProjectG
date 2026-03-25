// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AbilityTasks/SkillAbilityTask.h"

void USkillAbilityTask::EmitRuntimeEvent(const FGameplayTag& EventTag, const FGameplayAbilityTargetDataHandle& TargetData)
{
    if (EventTag.IsValid())
    {
        OnRuntimeEvent.Broadcast(EventTag, TargetData);
    }
}
