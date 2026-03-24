// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Lobby/Gacha/GachaResultWidget.h"
#include "Components/WidgetSwitcher.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "NiagaraSystemWidget.h"

void UGachaResultWidget::InitData(UUnitUIDataAsset* InData)
{
    if(InData) PickupUnit = InData;

    if (PickupUnit)
    {
        UnitImage->SetBrushFromTexture(PickupUnit->UnitImage);
    }
}

void UGachaResultWidget::PlayGachaAnim()
{
    PlayAnimation(UnitGacha);
    GachaEffect->ActivateSystem(false);
}

void UGachaResultWidget::NativeConstruct()
{
    if (SkipButton) SkipButton->OnClicked.AddDynamic(this, &UGachaResultWidget::OnSkipButtonClicked);
}

void UGachaResultWidget::OnSkipButtonClicked()
{
    if (WidgetSwitcher) WidgetSwitcher->SetActiveWidgetIndex(4);
}
