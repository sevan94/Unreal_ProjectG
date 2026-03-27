// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Lobby/Equip/SetEffectWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void USetEffectWidget::UpdateSetDisplay(UTexture2D* InIcon, FText InName, FText InDescription)
{
    if (SetImage)
    {
        if (InIcon)
        {
            SetImage->SetBrushFromTexture(InIcon);
            SetImage->SetVisibility(ESlateVisibility::Visible);
        }
        else
        {
            SetImage->SetVisibility(ESlateVisibility::Collapsed);
        }
    }
    
    if (SetName)
    {
        SetName->SetText(InName);
    }

    if (SetDescription)
    {
        SetDescription->SetText(InDescription);
    }
}
