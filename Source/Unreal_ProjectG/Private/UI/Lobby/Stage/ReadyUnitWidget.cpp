// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Lobby/Stage/ReadyUnitWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "DataAssets/UI/UnitUIDataAsset.h"

void UReadyUnitWidget::UpdateUnitWidget(TSoftObjectPtr<UUnitUIDataAsset> UnitDataAsset, int32 Level)
{
    if (UnitDataAsset && UnitImage)
    {
        UnitImage->SetBrushFromTexture(UnitDataAsset->UnitImage);
    }

    if (UnitLevel)
    {
        UnitLevel->SetText(FText::AsNumber(Level));
    }
}
