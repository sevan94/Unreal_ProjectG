// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Battle/BattleHUD.h"
#include "UI/Battle/BattleUIWidget.h"
#include "UI/Battle/ControlPanelWidget.h"

void ABattleHUD::OnGameOver(const FBattleResultData& ResultData)
{
    BattleUIWidget->ShowGameResult(ResultData);
}

void ABattleHUD::SetAbilitySpec()
{
    //if (BattleUIWidget->GetControlPanel())
    //{
    //    BattleUIWidget->GetControlPanel()->SetAbilitySpecHandle();
    //}
}

void ABattleHUD::BeginPlay()
{
    Super::BeginPlay();

    if (HUDWidgetClass)
    {
        UBattleUIWidget* BattleUI = CreateWidget<UBattleUIWidget>(GetOwningPlayerController(), HUDWidgetClass);
        if (BattleUI)
        {
            BattleUIWidget = BattleUI;
            BattleUI->AddToViewport();
        }
    }
}
