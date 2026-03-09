// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Lobby/Main/GoodsBarWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UGoodsBarWidget::InitializeGoodsBar(int32 InValue)
{
    if (GoodsIcon)
    {
        GoodsImage->SetBrushFromTexture(GoodsIcon);
    }
    
    GoodsText->SetText(FText::AsNumber(InValue));
}

void UGoodsBarWidget::UpdateGoodsText(int32 InValue)
{
    GoodsText->SetText(FText::AsNumber(InValue));
}
