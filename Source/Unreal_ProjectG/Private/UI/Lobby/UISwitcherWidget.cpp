// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Lobby/UISwitcherWidget.h"
#include "Components/WidgetSwitcher.h"
#include "UI/Lobby/MainLobbyWidget.h"
#include "UI/Lobby/LobbyUnitWidget.h"
#include "UI/Lobby/LobbyEquipWidget.h"
#include "UI/Lobby/LobbyStageWidget.h"
#include "UI/Lobby/LobbyGachaWidget.h"

void UUISwitcherWidget::NativeConstruct()
{
    if (WidgetSwitcher)
    {
        LobbyWidget->WidgetSwitcher = WidgetSwitcher;
        UnitWidget->WidgetSwitcher = WidgetSwitcher;
        EquipWidget->WidgetSwitcher = WidgetSwitcher;
        StageWidget->WidgetSwitcher = WidgetSwitcher;
        GachaWidget->WidgetSwitcher = WidgetSwitcher;

        // 기본 페이지 설정
        WidgetSwitcher->SetActiveWidgetIndex(0);
    }
}
