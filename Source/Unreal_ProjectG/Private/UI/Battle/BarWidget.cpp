// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Battle/BarWidget.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"

void UBarWidget::InitProgressBar(FLinearColor InColor, FText InName, float InMax)
{
    StatusBar->SetFillColorAndOpacity(InColor);
    StatusName->SetText(InName);
    MaxValue = InMax;
    Max->SetText(FText::AsNumber(FMath::RoundToInt(InMax)));
    Current->SetText(FText::AsNumber(FMath::RoundToInt(InMax)));

    StatusBar->SetPercent(MaxValue / MaxValue);
}

void UBarWidget::UpdateCurrent(float InCurrent)
{
    Current->SetText(FText::AsNumber(FMath::RoundToInt(InCurrent)));
    StatusBar->SetPercent(InCurrent / MaxValue);
}
