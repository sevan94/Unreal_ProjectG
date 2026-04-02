// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AbilityTasks/SkillAbilityTask_Presentation.h"

USkillAbilityTask_Presentation* USkillAbilityTask_Presentation::Create(UGameplayAbility* OwningAbility, UDataAsset_HeroSkillPresentation* PresentationData)
{
    APlayerController* PC = Cast<APlayerController>(OwningAbility->GetCurrentActorInfo()->PlayerController.Get());
    if (PC)
    {
        PC->PlayerCameraManager;
    }
}
