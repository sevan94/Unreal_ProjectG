// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Lobby/UISwitcherWidget.h"
#include "Components/WidgetSwitcher.h"

void UUISwitcherWidget::NativeConstruct()
{
    if (WidgetSwitcher)
    {
        // 위젯 스위처에 위젯 추가
        WidgetSwitcher->AddChild(LobbyWidget);
        WidgetSwitcher->AddChild(UnitWidget);
        WidgetSwitcher->AddChild(EquipWidget);
        WidgetSwitcher->AddChild(GachaWidget);
        WidgetSwitcher->AddChild(StageWidget);

        // 기본 페이지 설정
        WidgetSwitcher->SetActiveWidget(LobbyWidget);
    }
}
