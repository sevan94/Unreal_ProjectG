// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Lobby/Main/LobbyHUD.h"
#include "UI/Lobby/Main/UISwitcherWidget.h"

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
        }
    }
}
