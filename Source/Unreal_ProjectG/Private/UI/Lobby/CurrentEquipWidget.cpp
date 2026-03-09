// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Lobby/CurrentEquipWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "DataAssets/UI/EquipUIDataAsset.h"
#include "UI/Lobby/LobbyEquipWidget.h"

void UCurrentEquipWidget::UpdateEquipSlot(UEquipUIDataAsset* InData)
{
    if (InData)
    {
        // 아이템 아이콘 등으로 텍스처 업데이트 로직
        EquipImage->SetBrushFromTexture(InData->EquipImage);
        EquipImage->SetVisibility(ESlateVisibility::Visible);
    }
    else
    {
        // 장착된 아이템이 없을 때 빈 슬롯 처리
        EquipImage->SetVisibility(ESlateVisibility::Hidden);
    }
}

void UCurrentEquipWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (EquipButton)
    {
        EquipButton->OnClicked.AddDynamic(this, &UCurrentEquipWidget::OnEquipButtonClicked);
    }
}

void UCurrentEquipWidget::OnEquipButtonClicked()
{
    if (OnSelected.IsBound())
    {
        OnSelected.Broadcast(SlotCategory);
    }
}
