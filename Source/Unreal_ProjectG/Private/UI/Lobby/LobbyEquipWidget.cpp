// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Lobby/LobbyEquipWidget.h"
#include "Components/WidgetSwitcher.h"
#include "Components/Button.h"
#include "UI/Lobby/CurrentEquipWidget.h"
#include "UI/Lobby/EquipListWidget.h"
#include "UI/Lobby/EquipDescriptionWidget.h"
#include "DataAssets/UI/EquipUIDataAsset.h"
#include "Mode/Save/PGGameInstance.h"

void ULobbyEquipWidget::SetEquipList(EEquipCategory InType)
{
    CurrentActiveCategory = InType;
    EquipList->UpdateList(InType);

    SelectedEquip = nullptr;
    if (EquipButton)
    {
        EquipButton->SetIsEnabled(false);
    }
}

void ULobbyEquipWidget::NativeConstruct()
{
    if (ExitButton) ExitButton->OnClicked.AddDynamic(this, &ULobbyEquipWidget::OnExitButtonClick); 
    if (WeaponEquip) WeaponEquip->OnSelected.AddDynamic(this, &ULobbyEquipWidget::SetEquipList);
    if (ArmorEquip) ArmorEquip->OnSelected.AddDynamic(this, &ULobbyEquipWidget::SetEquipList);
    if (AccesoryEquip) AccesoryEquip->OnSelected.AddDynamic(this, &ULobbyEquipWidget::SetEquipList);
    if (EquipList) EquipList->OnEquipSelected.AddDynamic(this, &ULobbyEquipWidget::HandleEquipSelected);

    if (EquipButton)
    {
        EquipButton->SetIsEnabled(false);
        EquipButton->OnClicked.AddDynamic(this, &ULobbyEquipWidget::OnEquipButtonClicked);
    }

    EquipDescription->SetVisibility(ESlateVisibility::Hidden);
}

void ULobbyEquipWidget::OnExitButtonClick()
{
    EquipList->ClearTileView();
    SelectedEquip = nullptr;
    EquipButton->SetIsEnabled(false);
    EquipDescription->SetVisibility(ESlateVisibility::Hidden);
    if (WidgetSwitcher) WidgetSwitcher->SetActiveWidgetIndex(0);
}

void ULobbyEquipWidget::OnEquipButtonClicked()
{
    if (!SelectedEquip) return;

    UPGGameInstance* GI = Cast<UPGGameInstance>(GetGameInstance());

    // 현재 선택 카테고리에 따라 해당 부위 위젯 업데이트 및 인스턴스 저장
    switch (CurrentActiveCategory)
    {
    case EEquipCategory::Weapon:
        if (WeaponEquip) WeaponEquip->UpdateEquipSlot(SelectedEquip);
        GI->CurrentWeapon = SelectedEquip;
        break;
    case EEquipCategory::Armor:
        if (ArmorEquip) ArmorEquip->UpdateEquipSlot(SelectedEquip);
        GI->CurrentArmor = SelectedEquip->EquipDataAsset;
        break;
    case EEquipCategory::Accessory:
        if (AccesoryEquip) AccesoryEquip->UpdateEquipSlot(SelectedEquip);
        GI->CurrentAccessory = SelectedEquip->EquipDataAsset;
        break;
    }

    // 장착 후 처리
     EquipButton->SetIsEnabled(false);
     SelectedEquip = nullptr;
     EquipDescription->SetVisibility(ESlateVisibility::Hidden);
}

void ULobbyEquipWidget::HandleEquipSelected(UEquipUIDataAsset* InData)
{
    if(!InData) return;

    if (EquipDescription && InData)
    {
        // 정보 창 표시
        EquipDescription->SetVisibility(ESlateVisibility::Visible);
        EquipDescription->UpdateDescription(InData);
    }

    // 선택된 데이터 저장
    SelectedEquip = InData;

    // 장착 버튼 활성화
    if (EquipButton)
    {
        EquipButton->SetIsEnabled(true);
    }
}
