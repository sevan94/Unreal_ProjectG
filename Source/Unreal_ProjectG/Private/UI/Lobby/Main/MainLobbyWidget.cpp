// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Lobby/Main/MainLobbyWidget.h"
#include "Components/WidgetSwitcher.h"
#include "Components/Button.h"

void UMainLobbyWidget::NativeConstruct()
{
    // 버튼 클릭 이벤트 델리게이트 바인딩
    if (StageButton) StageButton->OnClicked.AddDynamic(this, &UMainLobbyWidget::OnStageButtonClick);
    if (UnitButton)  UnitButton->OnClicked.AddDynamic(this, &UMainLobbyWidget::OnUnitButtonClick);
    if (EquipButton) EquipButton->OnClicked.AddDynamic(this, &UMainLobbyWidget::OnEquipButtonClick);
    if (GachaButton) GachaButton->OnClicked.AddDynamic(this, &UMainLobbyWidget::OnGachaButtonClick);
}

void UMainLobbyWidget::OnUnitButtonClick()
{
    if (WidgetSwitcher) WidgetSwitcher->SetActiveWidgetIndex(1);
}

void UMainLobbyWidget::OnEquipButtonClick()
{
    if (WidgetSwitcher) WidgetSwitcher->SetActiveWidgetIndex(2);
}

void UMainLobbyWidget::OnStageButtonClick()
{
    if (WidgetSwitcher) WidgetSwitcher->SetActiveWidgetIndex(3);
}

void UMainLobbyWidget::OnGachaButtonClick()
{
    if (WidgetSwitcher) WidgetSwitcher->SetActiveWidgetIndex(4);
}

