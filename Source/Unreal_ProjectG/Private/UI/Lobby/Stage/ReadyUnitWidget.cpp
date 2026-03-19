// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Lobby/Stage/ReadyUnitWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "DataAssets/UI/UnitUIDataAsset.h"

void UReadyUnitWidget::UpdateUnitWidget(TSoftObjectPtr<UUnitUIDataAsset> InData, int32 Level)
{
    if (InData && UnitImage)
    {
        UnitImage->SetBrushFromTexture(InData->UnitImage);
    }

    if (UnitLevel)
    {
        UnitLevel->SetText(FText::AsNumber(Level));
    }
}
