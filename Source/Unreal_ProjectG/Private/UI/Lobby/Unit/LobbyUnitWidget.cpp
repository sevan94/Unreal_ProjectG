// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Lobby/Unit/LobbyUnitWidget.h"
#include "Components/WidgetSwitcher.h"
#include "Components/Button.h"
#include "UI/Lobby/Unit/UnitListWidget.h"
#include "UI/Lobby/Unit/UnitDescriptionWidget.h"
#include "UI/Lobby/Unit/CurrentUnitWidget.h"
#include "UI/UnitEntryObject.h"
#include "UI/Lobby/Main/GoodsBarWidget.h"
#include "DataAssets/UI/UnitUIDataAsset.h"
#include "Components/HorizontalBox.h"
#include "Mode/Save/PGGameInstance.h"

void ULobbyUnitWidget::NativeConstruct()
{
    Super::NativeConstruct();
    PartySlots.Empty();
    GI = Cast<UPGGameInstance>(GetGameInstance());
    if (GI)
    {
        GI->LoadGameData();
        GI->OnGoodsChanged.AddDynamic(this, &ULobbyUnitWidget::UpdateGoodsBar);

        Unlock->InitializeGoodsBar(GI->CurrentPlayerUnlock);
        Gold->InitializeGoodsBar(GI->CurrentPlayerGold);
    }
    UnitDescription->SetVisibility(ESlateVisibility::Hidden);

    InitializePartySlots();

    if (ExitButton) ExitButton->OnClicked.AddDynamic(this, &ULobbyUnitWidget::OnExitButtonClick);
    if (UnitList) UnitList->OnUnitSelected.AddDynamic(this, &ULobbyUnitWidget::HandleUnitSelected);
}

void ULobbyUnitWidget::HandlePartySlotClick(int32 SlotIndex)
{
    if (SelectedUnit && PartySlots.IsValidIndex(SlotIndex))
    {

        // 선택한 유닛이 이미 다른 슬롯에 있는지 확인 (중복 검사)
        UCurrentUnitWidget* ExistingSlot = nullptr;
        for (UCurrentUnitWidget* PartySlot : PartySlots)
        {
            if (PartySlot && PartySlot->GetUnitData() == SelectedUnit)
            {
                ExistingSlot = PartySlot;
                break;
            }
        }

        // 자리 교체 로직
        if (ExistingSlot)
        {
            // 중복된 유닛이 있다면 해당 유닛과 자리 스왑
            UUnitUIDataAsset* TargetSlotData = PartySlots[SlotIndex]->GetUnitData();
            ExistingSlot->UpdateSlot(TargetSlotData);
        }

        PartySlots[SlotIndex]->UpdateSlot(SelectedUnit);

        // 인스턴스에 최종 변경사항 저장
        if (GI)
        {
            TArray<TSoftObjectPtr<UUnitUIDataAsset>> NewPartyList;
            for (UCurrentUnitWidget* PartySlot : PartySlots)
            {
                NewPartyList.Add(TSoftObjectPtr<UUnitUIDataAsset>(PartySlot->GetUnitData()));
            }
            GI->CurrentUnits = NewPartyList;
            GI->SaveGameData();
        }

        // 교체 완료 후 오버레이 모두 끄기
        for (auto PartySlot : PartySlots)
        {
            if (PartySlot) PartySlot->ShowReplaceOverlay(false);
        }

        // 대기 유닛 초기화
        SelectedUnit = nullptr;

        // 설명 창 숨기기
        UnitDescription->SetVisibility(ESlateVisibility::Hidden);
    }
}

void ULobbyUnitWidget::InitializePartySlots()
{
    if (PartyBox)
    {
        for (int32 i = 0; i < PartyBox->GetChildrenCount(); ++i)
        {
            if (UCurrentUnitWidget* PartySlot = Cast<UCurrentUnitWidget>(PartyBox->GetChildAt(i)))
            {
                PartySlot->SlotIndex = i; // 인덱스 자동 부여
                PartySlot->OnSlotClicked.AddDynamic(this, &ULobbyUnitWidget::HandlePartySlotClick);
                PartySlots.Add(PartySlot);
                if (GI->CurrentUnits.IsValidIndex(i))
                    PartySlot->UpdateSlot(GI->CurrentUnits[i].LoadSynchronous());
            }
        }
    }
}

void ULobbyUnitWidget::UpdateGoodsBar(EGoodsCategory InCategory, int32 InValue)
{
    switch (InCategory)
    {
    case EGoodsCategory::Unlock: Unlock->UpdateGoodsText(InValue); break;
    case EGoodsCategory::Gold: Gold->UpdateGoodsText(InValue); break;
    }
}

void ULobbyUnitWidget::OnExitButtonClick()
{
    SelectedUnit = nullptr;
    UnitDescription->SetVisibility(ESlateVisibility::Hidden);
    if (WidgetSwitcher) WidgetSwitcher->SetActiveWidgetIndex(0);
}

void ULobbyUnitWidget::HandleUnitSelected(UUnitEntryObject* InData)
{
    if (!UnitDescription || !InData) return;

    // 정보 창 표시
    UnitDescription->SetVisibility(ESlateVisibility::Visible);
    UnitDescription->UpdateDescription(InData);

    // 보유 여부 확인
    FUnitSaveData SaveData = GI->GetUnitSaveData(InData->GetUnitUIData()->UnitID);

    if (SaveData.bIsUnlocked)
    {
        // 교체 대기 유닛 저장
        SelectedUnit = InData->GetUnitUIData();

        // 모든 파티 슬롯에 교체 오버레이 켜기
        for (auto PartySlot : PartySlots)
        {
            if (PartySlot) PartySlot->ShowReplaceOverlay(true);
        }
    }
    else
    {
        // 교체 대기 유닛 해제
        SelectedUnit = nullptr;

        // 모든 파티 슬롯에 교체 오버레이 끄기
        for (auto PartySlot : PartySlots)
        {
            if (PartySlot) PartySlot->ShowReplaceOverlay(false);
        }
    }
}
