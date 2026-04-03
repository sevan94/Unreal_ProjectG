// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Battle/UnitPanelWidget.h"
#include "UI/Battle/UnitSlotWidget.h"
#include "Mode/Save/PGGameInstance.h"

void UUnitPanelWidget::UpdateAllSlots(float InCost)
{
    for (UUnitSlotWidget* UnitSlot : SlotArray)
    {
        UnitSlot->UpdateSlot(InCost);
    }
}

void UUnitPanelWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // 슬롯 추가
    SlotArray.Empty();
    if (UnitSlot1) SlotArray.Add(UnitSlot1);
    if (UnitSlot2) SlotArray.Add(UnitSlot2);
    if (UnitSlot3) SlotArray.Add(UnitSlot3);
    if (UnitSlot4) SlotArray.Add(UnitSlot4);
    if (UnitSlot5) SlotArray.Add(UnitSlot5);

    // --- [저장된 유닛 데이터 로드] ---
    if (UPGGameInstance* GI = Cast<UPGGameInstance>(GetGameInstance()))
    {
        UnitDataList.Empty();
        for (const TSoftObjectPtr<UUnitUIDataAsset>& UnitSoftPtr : GI->CurrentUnits)
        {
            // SoftObjectPtr을 실제 에셋 포인터로 로드
            if (UUnitUIDataAsset* LoadedData = UnitSoftPtr.LoadSynchronous())
            {
                UnitDataList.Add(LoadedData);
            }
        }
    }

    SetUnitSlot();
}

void UUnitPanelWidget::SetUnitSlot()
{
    int32 RandomLockedIndex = FMath::RandRange(0, SlotArray.Num() - 1);

    for (int32 i = 0; i < SlotArray.Num(); ++i)
    {
        if (UnitDataList.IsValidIndex(i) && UnitDataList[i])
        {
            bool bShouldLock = (i == RandomLockedIndex);

            SlotArray[i]->InitializeSlot(UnitDataList[i], bShouldLock);
            SlotArray[i]->SetVisibility(ESlateVisibility::Visible);
        }
        else
        {
            SlotArray[i]->SetVisibility(ESlateVisibility::Collapsed);
        }
    }
}
