// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Battle/BaseHpWidget.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"

void UBaseHpWidget::UpdateMaxHP(float InMax)
{
    MaxValue = InMax;
    MaxHP->SetText(FText::AsNumber(FMath::RoundToInt(InMax)));
    CurrentHP->SetText(FText::AsNumber(FMath::RoundToInt(InMax)));
    HPBar->SetPercent(MaxValue / MaxValue);
}

void UBaseHpWidget::UpdateCurrentHP(float InCurrent)
{
    CurrentHP->SetText(FText::AsNumber(FMath::RoundToInt(InCurrent)));
    HPBar->SetPercent(InCurrent / MaxValue);
}
