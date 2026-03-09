// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Unit/Component/ElementIconComponent.h"
#include "Character/Unit/UnitCharacter.h"

void UElementIconComponent::BeginPlay()
{
    Super::BeginPlay();

    if (AUnitCharacter* UnitOwner = Cast<AUnitCharacter>(GetOwner()))
    {
        UnitOwner->OnUnitStartUpDataLoadedDelegate.AddUObject(this, &UElementIconComponent::HandleUnitDataLoaded);
    }
}

UElementIconComponent::UElementIconComponent()
{
    bHiddenInGame = false;
}

void UElementIconComponent::HandleUnitDataLoaded()
{
    if (AUnitCharacter* UnitOwner = Cast<AUnitCharacter>(GetOwner()))
    {
        FGameplayTag CurrentBranch = UnitOwner->GetBranchTag();
        FGameplayTag CurrentElement = UnitOwner->GetElementTag();

        UpdateOverheadIcon(CurrentBranch, CurrentElement);
    }
}

void UElementIconComponent::UpdateOverheadIcon(FGameplayTag CurrentBranch, FGameplayTag CurrentElement)
{
    for (const FUnitIconData& IconData : IconMappings)
    {
        if (IconData.BranchTag == CurrentBranch && IconData.ElementTag == CurrentElement)
        {
            SetSprite(IconData.IconTexture);
            return;
        }
    }

    SetSprite(nullptr);
}
