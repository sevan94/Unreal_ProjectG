// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Lobby/Stage/LobbyStageWidget.h"
#include "Components/WidgetSwitcher.h"
#include "Components/Button.h"
#include "UI/Lobby/Stage/StageMapWidget.h"

void ULobbyStageWidget::NativeConstruct()
{
    if (ExitButton) ExitButton->OnClicked.AddDynamic(this, &ULobbyStageWidget::OnExitButtonClick);

    StageMap->RefreshStageMap();
}

void ULobbyStageWidget::OnExitButtonClick()
{
    if (WidgetSwitcher) WidgetSwitcher->SetActiveWidgetIndex(0);
}

