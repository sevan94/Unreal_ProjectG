// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Lobby/Main/LobbyHUD.h"
#include "UI/Lobby/Main/UISwitcherWidget.h"
#include "UI/Lobby/Gacha/GachaResultWidget.h"
#include "Components/WidgetSwitcher.h"
#include "Mode/Save/PGGameInstance.h"

void ALobbyHUD::ShowUnitGachaResultUI()
{
    if (UISwitcherInstance && UISwitcherInstance->WidgetSwitcher)
    {
        // UISwitcherWidget 내부의 함수를 호출하여 결과창 핸들링
        UGachaResultWidget* ResultWidget = Cast<UGachaResultWidget>(UISwitcherInstance->WidgetSwitcher->GetWidgetAtIndex(5));
        if (ResultWidget)
        {
            ResultWidget->PlayGachaAnim();
        }
    }
}

void ALobbyHUD::ShowEquipGachaResultUI()
{
    if (UISwitcherInstance && UISwitcherInstance->WidgetSwitcher)
    {
        // UISwitcherWidget 내부의 함수를 호출하여 결과창 핸들링
        UGachaResultWidget* ResultWidget = Cast<UGachaResultWidget>(UISwitcherInstance->WidgetSwitcher->GetWidgetAtIndex(5));
        if (ResultWidget)
        {
            ResultWidget->ShowEquipResult();
        }
    }
}

void ALobbyHUD::BeginPlay()
{
    Super::BeginPlay();

    // 위젯 생성 및 뷰포트 추가
    if (UISwitcher)
    {
        UISwitcherInstance = CreateWidget<UUISwitcherWidget>(GetWorld(), UISwitcher);
        if (UISwitcher)
        {
            UISwitcherInstance->AddToViewport();
            UPGGameInstance* GI = Cast<UPGGameInstance>(GetGameInstance());
            if (GI && GI->bStageSelect)
            {
                UISwitcherInstance->WidgetSwitcher->SetActiveWidgetIndex(3);
                GI->bStageSelect = false;
            }
        }
    }
}
