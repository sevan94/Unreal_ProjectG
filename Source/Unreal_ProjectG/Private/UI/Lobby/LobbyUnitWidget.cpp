// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Lobby/LobbyUnitWidget.h"
#include "Components/WidgetSwitcher.h"
#include "Components/Button.h"
#include "UI/Lobby/UnitListWidget.h"
#include "UI/Lobby/UnitDescriptionWidget.h"

void ULobbyUnitWidget::NativeConstruct()
{
    if (ExitButton) ExitButton->OnClicked.AddDynamic(this, &ULobbyUnitWidget::OnExitButtonClick);
    if (UnitList) UnitList->OnUnitSelected.AddDynamic(this, &ULobbyUnitWidget::HandleUnitSelected);
}

void ULobbyUnitWidget::OnExitButtonClick()
{
    if (WidgetSwitcher) WidgetSwitcher->SetActiveWidgetIndex(0);
}

void ULobbyUnitWidget::HandleUnitSelected(UUnitUIDataAsset* SelectedData)
{
    if (UnitDescription && SelectedData)
    {
        // 정보창(Description)의 함수를 직접 호출하여 데이터를 넘겨줍니다.
        UnitDescription->UpdateDescription(SelectedData);
    }
}
