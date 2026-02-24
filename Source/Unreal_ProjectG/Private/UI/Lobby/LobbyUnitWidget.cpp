// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Lobby/LobbyUnitWidget.h"
#include "Components/WidgetSwitcher.h"
#include "Components/Button.h"

void ULobbyUnitWidget::NativeConstruct()
{
    if (ExitButton) ExitButton->OnClicked.AddDynamic(this, &ULobbyUnitWidget::OnExitButtonClick);
}

void ULobbyUnitWidget::OnExitButtonClick()
{
    if (WidgetSwitcher) WidgetSwitcher->SetActiveWidgetIndex(0);
}
