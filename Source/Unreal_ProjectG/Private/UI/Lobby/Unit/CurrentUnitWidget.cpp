// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Lobby/Unit/CurrentUnitWidget.h"
#include "DataAssets/UI/UnitUIDataAsset.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/Overlay.h"

void UCurrentUnitWidget::UpdateSlot(UUnitUIDataAsset* NewData)
{
    CurrentUnitData = NewData;

    if (UnitImage && CurrentUnitData)
    {
        UnitImage->SetBrushFromTexture(CurrentUnitData->UnitImage);
        UnitImage->SetVisibility(ESlateVisibility::Visible);
    }
    else if (UnitImage)
    {
        // 데이터가 없는 경우 투명하게 하거나 기본 이미지 처리
        UnitImage->SetVisibility(ESlateVisibility::Hidden);
    }
}

void UCurrentUnitWidget::ShowReplaceOverlay(bool bVisible)
{
    if (ReplaceOverlay)
    {
        ReplaceOverlay->SetVisibility(bVisible ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);
    }
}

void UCurrentUnitWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (UnitButton)
    {
        UnitButton->OnClicked.AddDynamic(this, &UCurrentUnitWidget::OnCurrentButtonClicked);
    }

    UpdateSlot(CurrentUnitData);

    // 초기 상태에서는 교체 오버레이를 숨김
    ShowReplaceOverlay(false);
}

void UCurrentUnitWidget::OnCurrentButtonClicked()
{
    if (OnSlotClicked.IsBound())
    {
        OnSlotClicked.Broadcast(SlotIndex);
    }
}
