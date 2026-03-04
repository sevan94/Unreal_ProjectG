// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Lobby/LobbyEquipWidget.h"
#include "Components/WidgetSwitcher.h"
#include "Components/Button.h"
#include "UI/Lobby/CurrentEquipWidget.h"
#include "UI/Lobby/EquipListWidget.h"

void ULobbyEquipWidget::SetEquipList(EEquipCategory InType)
{
    EquipList->UpdateList(InType);
}

void ULobbyEquipWidget::NativeConstruct()
{
    if (ExitButton) ExitButton->OnClicked.AddDynamic(this, &ULobbyEquipWidget::OnExitButtonClick); 
    if (WeaponEquip) WeaponEquip->OnSelected.AddDynamic(this, &ULobbyEquipWidget::SetEquipList);
    if (ArmorEquip) ArmorEquip->OnSelected.AddDynamic(this, &ULobbyEquipWidget::SetEquipList);
    if (AccesoryEquip) AccesoryEquip->OnSelected.AddDynamic(this, &ULobbyEquipWidget::SetEquipList);
}

void ULobbyEquipWidget::OnExitButtonClick()
{
    if (WidgetSwitcher) WidgetSwitcher->SetActiveWidgetIndex(0);
}
