// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Lobby/LobbyEquipWidget.h"
#include "Components/WidgetSwitcher.h"
#include "Components/Button.h"

void ULobbyEquipWidget::NativeConstruct()
{
    if (ExitButton) ExitButton->OnClicked.AddDynamic(this, &ULobbyEquipWidget::OnExitButtonClick);
}

void ULobbyEquipWidget::OnExitButtonClick()
{
    if (WidgetSwitcher) WidgetSwitcher->SetActiveWidgetIndex(0);
}

