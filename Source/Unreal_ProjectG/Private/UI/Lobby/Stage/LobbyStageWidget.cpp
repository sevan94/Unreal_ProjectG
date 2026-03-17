// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Lobby/Stage/LobbyStageWidget.h"
#include "Components/WidgetSwitcher.h"
#include "Components/Button.h"
#include "UI/Lobby/Stage/StageMapWidget.h"
#include "UI/Lobby/Stage/StageReadyWidget.h"
#include "Mode/Save/PGGameInstance.h"

void ULobbyStageWidget::NativeConstruct()
{
    if (ExitButton) ExitButton->OnClicked.AddDynamic(this, &ULobbyStageWidget::OnExitButtonClick);

    if (StageMap)
    {
        StageMap->RefreshStageMap();
        StageMap->OnRequestStageInfo.AddDynamic(this, &ULobbyStageWidget::OpenStageInfo);
    }
}

void ULobbyStageWidget::OpenStageInfo(int32 StageCode)
{
    UPGGameInstance* GI = GetGameInstance<UPGGameInstance>();
    if (GI && StageReady)
    {
        // 게임 인스턴스에 스테이지 코드 저장
        GI->SelectedStageNum = StageCode;

        // 정보창 보이기 및 갱신
        StageReady->SetVisibility(ESlateVisibility::Visible);
        StageReady->InitializeReadyWidget();

    }
}

void ULobbyStageWidget::OnExitButtonClick()
{
    if (WidgetSwitcher) WidgetSwitcher->SetActiveWidgetIndex(0);
}

