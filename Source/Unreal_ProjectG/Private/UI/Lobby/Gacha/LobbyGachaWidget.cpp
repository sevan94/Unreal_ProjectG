// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Lobby/Gacha/LobbyGachaWidget.h"
#include "Components/WidgetSwitcher.h"
#include "Components/Button.h"

void ULobbyGachaWidget::NativeConstruct()
{
    if (ExitButton) ExitButton->OnClicked.AddDynamic(this, &ULobbyGachaWidget::OnExitButtonClick);
}

void ULobbyGachaWidget::OnExitButtonClick()
{
    if (WidgetSwitcher) WidgetSwitcher->SetActiveWidgetIndex(0);
}