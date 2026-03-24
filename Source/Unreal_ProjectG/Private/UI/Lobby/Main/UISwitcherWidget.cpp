// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Lobby/Main/UISwitcherWidget.h"
#include "Components/WidgetSwitcher.h"
#include "UI/Lobby/Main/MainLobbyWidget.h"
#include "UI/Lobby/Unit/LobbyUnitWidget.h"
#include "UI/Lobby/Equip/LobbyEquipWidget.h"
#include "UI/Lobby/Stage/LobbyStageWidget.h"
#include "UI/Lobby/Gacha/LobbyGachaWidget.h"
#include "UI/Lobby/Gacha/GachaResultWidget.h"

void UUISwitcherWidget::NativeConstruct()
{
    if (WidgetSwitcher)
    {
        LobbyWidget->WidgetSwitcher = WidgetSwitcher;
        UnitWidget->WidgetSwitcher = WidgetSwitcher;
        EquipWidget->WidgetSwitcher = WidgetSwitcher;
        StageWidget->WidgetSwitcher = WidgetSwitcher;
        GachaWidget->WidgetSwitcher = WidgetSwitcher;
        GachaResultWidget->WidgetSwitcher = WidgetSwitcher;

        // 기본 페이지 설정
        WidgetSwitcher->SetActiveWidgetIndex(0);
    }
}
