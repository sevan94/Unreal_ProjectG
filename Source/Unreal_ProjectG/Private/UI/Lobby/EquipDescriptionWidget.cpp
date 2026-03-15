// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Lobby/EquipDescriptionWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "DataAssets/UI/EquipUIDataAsset.h"

void UEquipDescriptionWidget::UpdateDescription(UEquipUIDataAsset* InData)
{
    if (!InData) return;

    SkillImage1->SetBrushFromTexture(InData->SkillImage1);
    SkillImage2->SetBrushFromTexture(InData->SkillImage2);
    SkillDescription1->SetText(FText::FromString(InData->SkillDescription1));
    SkillDescription2->SetText(FText::FromString(InData->SkillDescription2));
}
