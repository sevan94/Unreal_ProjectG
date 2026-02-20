// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Battle/UnitPanelWidget.h"
#include "UI/Battle/UnitSlotWidget.h"

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

    // UnitDataAsset 전달
    for (int32 i = 0; i < SlotArray.Num(); ++i)
    {
        // 오류 검사
        if (UnitDataList.IsValidIndex(i) && UnitDataList[i])
        {
            // 유닛 슬롯 초기화 함수 호출
            SlotArray[i]->InitializeSlot(UnitDataList[i]);
            SlotArray[i]->SetVisibility(ESlateVisibility::Visible);
        }
        else
        {
            // 데이터가 없는 슬롯은 숨기거나 비활성화 처리
            SlotArray[i]->SetVisibility(ESlateVisibility::Collapsed);
        }
    }
}
