// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Lobby/Stage/ReadyEnemyWidget.h"
#include "Components/Image.h"
#include "DataAssets/UI/EnemyUIDataAsset.h"

void UReadyEnemyWidget::UpdateEnemyWidget(UEnemyUIDataAsset* InData)
{
    if (InData && EnemyImage)
    {
        EnemyImage->SetBrushFromTexture(InData->EnemyImage);
    }
}
