// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Lobby/Stage/StageMapWidget.h"
#include "UI/Lobby/Stage/StageButtonWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"

void UStageMapWidget::RefreshStageMap()
{
    if (MapContainer)
    {
        MapContainer->ClearChildren();
        GenerateStageButtons();
    }
}

void UStageMapWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // 생성 시점에 스테이지 버튼들을 배치
    GenerateStageButtons();
}

void UStageMapWidget::GenerateStageButtons()
{
    if (!StageDataTable || !StageButtonClass || !MapContainer) return;

    static const FString ContextString(TEXT("Stage Generation Context"));
    TArray<FStageDataTable*> StageRows;
    StageDataTable->GetAllRows<FStageDataTable>(ContextString, StageRows);

    for (FStageDataTable* Data : StageRows)
    {
        // 버튼 위젯 생성
        UStageButtonWidget* NewButton = CreateWidget<UStageButtonWidget>(this, StageButtonClass);
        if (NewButton)
        {
            // 버튼 초기화
            NewButton->InitStageButton(Data->StageNumber, Data->StarCount);

            // 캔버스 패널(MapContainer)에 추가
            UCanvasPanelSlot* ButtonSlot = MapContainer->AddChildToCanvas(NewButton);
            if (ButtonSlot)
            {
                // 데이터에 저장된 위치로 배치
                ButtonSlot->SetAlignment(FVector2D(0.5f, 0.5f));
                ButtonSlot->SetPosition(Data->MapPosition);

                // 버튼 크기가 찌그러지지 않게 크기에 맞춤 설정
                ButtonSlot->SetAutoSize(true);
            }
        }
    }
}
