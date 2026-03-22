// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AbilityTasks/SkillAbilityTask_MeleeTrace.h"
#include "AbilitySystem/Abilities/PGGameplayAbility.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Character/Hero/HeroCharacter.h"
#include "Components/Equipment/EquipmentStorageComponent.h"
#include "PGGameplayTags.h"

namespace MeleeTraceConstants
{
    // 트레이스 반복 간격
    // 0.016 ≈ 60fps 기준 1프레임
    constexpr float TraceInterval = 0.016f;
}

USkillAbilityTask_MeleeTrace* USkillAbilityTask_MeleeTrace::Create(UGameplayAbility* OwningAbility, const FSkillActionRow& ActionRow)
{
    UPGSkillTask_MeleeTrace* Task = NewAbilityTask<UPGSkillTask_MeleeTrace>(OwningAbility);
    Task->CachedActionRow = ActionRow;
    return Task;
}

void USkillAbilityTask_MeleeTrace::Activate()
{
    const FMeleeTraceConfig& Config = CachedActionRow.MeleeTraceConfig;
}

void USkillAbilityTask_MeleeTrace::OnDestroy(bool bInOwnerFinished)
{

}

void USkillAbilityTask_MeleeTrace::OnMontageCancelled()
{

}

void USkillAbilityTask_MeleeTrace::OnTraceStartEventReceived(FGameplayEventData Payload)
{

}

void USkillAbilityTask_MeleeTrace::OnTraceEndEventReceived(FGameplayEventData Payload)
{

}

void USkillAbilityTask_MeleeTrace::ExecuteTrace()
{

}