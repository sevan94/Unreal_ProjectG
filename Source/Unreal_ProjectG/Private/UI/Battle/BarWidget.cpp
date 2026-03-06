// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Battle/BarWidget.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Components/Image.h"

void UBarWidget::InitProgressBar(UTexture2D* Icon, FLinearColor InColor, FText InName)
{
    StatusBar->SetFillColorAndOpacity(InColor);
    StatusName->SetText(InName);
    StatusIcon->SetBrushFromTexture(Icon);
}

void UBarWidget::UpdateCurrent(float InCurrent)
{
    Current->SetText(FText::AsNumber(FMath::RoundToInt(InCurrent)));
    StatusBar->SetPercent(InCurrent / MaxValue);
}

void UBarWidget::UpdateMax(float InMax)
{
    MaxValue = InMax;
    Max->SetText(FText::AsNumber(FMath::RoundToInt(InMax)));
    Current->SetText(FText::AsNumber(FMath::RoundToInt(InMax)));
    StatusBar->SetPercent(MaxValue / MaxValue);
}
