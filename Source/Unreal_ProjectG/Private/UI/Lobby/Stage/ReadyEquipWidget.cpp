// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Lobby/Stage/ReadyEquipWidget.h"
#include "DataAssets/UI/EquipUIDataAsset.h"
#include "Components/Image.h"

void UReadyEquipWidget::UpdateEquipWidget(UEquipUIDataAsset* InData)
{
    if (EquipImage && InData)
    {
        EquipImage->SetBrushFromTexture(InData->EquipImage);
    }
}
