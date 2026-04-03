// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Lobby/Equip/EquipDescriptionWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Overlay.h"
#include "DataAssets/UI/EquipUIDataAsset.h"

void UEquipDescriptionWidget::UpdateDescription(UEquipUIDataAsset* InData)
{
    if (!InData) return;

    SkillImage1->SetBrushFromTexture(InData->SkillImage1);
    if (InData->SkillImage2)
    {
        Skill2ImagePanel->SetVisibility(ESlateVisibility::Visible);
        SkillImage2->SetBrushFromTexture(InData->SkillImage2);
    }
    else
    {
        Skill2ImagePanel->SetVisibility(ESlateVisibility::Hidden);
    }
    SkillDescription1->SetText(FText::FromString(InData->SkillDescription1));
    SkillDescription2->SetText(FText::FromString(InData->SkillDescription2));
}
