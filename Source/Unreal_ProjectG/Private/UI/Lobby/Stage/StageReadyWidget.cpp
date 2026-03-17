// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Lobby/Stage/StageReadyWidget.h"
#include "Components/Button.h"
#include "Mode/Save/PGGameInstance.h"
#include "DataAssets/UI/UnitUIDataAsset.h"
#include "UI/Lobby/Stage/ReadyUnitWidget.h"

void UStageReadyWidget::NativeConstruct()
{
    Super::NativeConstruct();

    GI = Cast<UPGGameInstance>(GetGameInstance());
    InitializeReadyWidget();

    if (ExitButton) ExitButton->OnClicked.AddDynamic(this, &UStageReadyWidget::OnExitButtonClicked);
}

void UStageReadyWidget::OnExitButtonClicked()
{
    this->SetVisibility(ESlateVisibility::Hidden);
}

void UStageReadyWidget::InitializeReadyWidget()
{
    TArray<TObjectPtr<UReadyUnitWidget>> UnitWidgets;
    UnitWidgets.Add(ReadyUnit1);
    UnitWidgets.Add(ReadyUnit2);
    UnitWidgets.Add(ReadyUnit3);
    UnitWidgets.Add(ReadyUnit4);
    UnitWidgets.Add(ReadyUnit5);

    for (int32 i = 0; i < GI->CurrentUnits.Num(); ++i)
    {
        if (!UnitWidgets.IsValidIndex(i) || !UnitWidgets[i]) break;

        UUnitUIDataAsset* UnitData = GI->CurrentUnits[i].LoadSynchronous();
        if (UnitData)
        {
            // UnitID를 통해 저장된 레벨 정보 가져오기
            int32 Level = 1;
            if (GI->UnitLevelMap.Contains(UnitData->UnitID))
            {
                Level = GI->UnitLevelMap[UnitData->UnitID].Level;
            }

            UnitWidgets[i]->UpdateUnitWidget(UnitData, Level);
        }
    }
}
