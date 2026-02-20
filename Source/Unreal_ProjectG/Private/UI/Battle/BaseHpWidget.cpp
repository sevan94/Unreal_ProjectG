// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Battle/BaseHpWidget.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"

void UBaseHpWidget::InitHPBar(float InCurrent, float InMax)
{
    MaxValue = InMax;
    CurrentValue = InCurrent;
    MaxHP->SetText(FText::AsNumber(FMath::RoundToInt(InMax)));
    CurrentHP->SetText(FText::AsNumber(FMath::RoundToInt(InCurrent)));

    HPBar->SetPercent(CurrentValue / MaxValue);
}

void UBaseHpWidget::UpdateCurrentHP(float InCurrent)
{
    CurrentHP->SetText(FText::AsNumber(FMath::RoundToInt(InCurrent)));
    HPBar->SetPercent(InCurrent / MaxValue);
}
