// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Lobby/LobbyStageWidget.h"
#include "Components/WidgetSwitcher.h"
#include "Components/Button.h"

void ULobbyStageWidget::NativeConstruct()
{
    if (ExitButton) ExitButton->OnClicked.AddDynamic(this, &ULobbyStageWidget::OnExitButtonClick);
}

void ULobbyStageWidget::OnExitButtonClick()
{
    if (WidgetSwitcher) WidgetSwitcher->SetActiveWidgetIndex(0);
}

