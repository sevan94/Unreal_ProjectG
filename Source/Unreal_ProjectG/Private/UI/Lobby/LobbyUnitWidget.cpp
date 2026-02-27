// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Lobby/LobbyUnitWidget.h"
#include "Components/WidgetSwitcher.h"
#include "Components/Button.h"
#include "UI/Lobby/UnitListWidget.h"
#include "UI/Lobby/UnitDescriptionWidget.h"
#include "UI/Lobby/PartyUnitWidget.h"
#include "Components/HorizontalBox.h"

void ULobbyUnitWidget::NativeConstruct()
{
    Super::NativeConstruct();
    PartySlots.Empty();

    if (PartyBox)
    {
        for (int32 i = 0; i < PartyBox->GetChildrenCount(); ++i)
        {
            if (UPartyUnitWidget* PartySlot = Cast<UPartyUnitWidget>(PartyBox->GetChildAt(i)))
            {
                PartySlot->SlotIndex = i; // 인덱스 자동 부여
                PartySlot->OnSlotClicked.AddDynamic(this, &ULobbyUnitWidget::HandlePartySlotClick);
                PartySlots.Add(PartySlot);
            }
        }
    }

    if (ExitButton) ExitButton->OnClicked.AddDynamic(this, &ULobbyUnitWidget::OnExitButtonClick);
    if (UnitList) UnitList->OnUnitSelected.AddDynamic(this, &ULobbyUnitWidget::HandleUnitSelected);
}

void ULobbyUnitWidget::HandlePartySlotClick(int32 SlotIndex)
{
    if (SelectedUnit && PartySlots.IsValidIndex(SlotIndex))
    {
        // 해당 슬롯의 유닛 데이터를 교체
        PartySlots[SlotIndex]->UpdateSlot(SelectedUnit);

        // 2. 교체 완료 후 오버레이 모두 끄기
        for (auto PartySlot : PartySlots)
        {
            if (PartySlot) PartySlot->ShowReplaceOverlay(false);
        }

        // 대기 유닛 초기화
        SelectedUnit = nullptr;
    }
}

void ULobbyUnitWidget::OnExitButtonClick()
{
    if (WidgetSwitcher) WidgetSwitcher->SetActiveWidgetIndex(0);
}

void ULobbyUnitWidget::HandleUnitSelected(UUnitUIDataAsset* SelectedData)
{
    if (UnitDescription && SelectedData)
    {
        // 정보 창 표시
        UnitDescription->UpdateDescription(SelectedData);
    }

    // 교체 대기 유닛 저장
    SelectedUnit = SelectedData;
    UE_LOG(LogTemp, Log, TEXT("교체 유닛 저장"));

    // 모든 파티 슬롯에 교체 오버레이 켜기
    for (auto PartySlot : PartySlots)
    {
        if (PartySlot) PartySlot->ShowReplaceOverlay(true);
    }
}
