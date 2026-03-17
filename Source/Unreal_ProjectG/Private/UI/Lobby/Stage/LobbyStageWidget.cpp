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
    UDataTable* DataTable = StageMap->GetStageData();
    if (!DataTable || !StageReady) return;

    static const FString ContextString(TEXT("Stage Info"));
    TArray<FStageDataTable*> StageRows;
    DataTable->GetAllRows<FStageDataTable>(ContextString, StageRows);

    // 전달받은 StageCode와 일치하는 데이터 탐색
    for (FStageDataTable* Row : StageRows)
    {
        if (Row && Row->StageCode == StageCode)
        {
            // GameInstance에 현재 선택한 스테이지 코드 저장
            if (UPGGameInstance* GI = GetGameInstance<UPGGameInstance>())
            {
                GI->SelectedStageNum = StageCode;
            }

            // 적 리스트 전달 및 UI 갱신
            StageReady->InitializeReadyWidget(Row->EnemyList, Row->StageLevel);

            // 정보창 표시
            StageReady->SetVisibility(ESlateVisibility::Visible);
            break;
        }
    }
}

void ULobbyStageWidget::OnExitButtonClick()
{
    if (WidgetSwitcher) WidgetSwitcher->SetActiveWidgetIndex(0);
}

