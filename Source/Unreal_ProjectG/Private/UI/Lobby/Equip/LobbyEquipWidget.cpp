// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Lobby/Equip/LobbyEquipWidget.h"
#include "Components/WidgetSwitcher.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "UI/Lobby/Equip/HeroStatusWidget.h"
#include "UI/Lobby/Equip/CurrentEquipWidget.h"
#include "UI/Lobby/Equip/EquipListWidget.h"
#include "UI/Lobby/Equip/EquipDescriptionWidget.h"
#include "UI/Lobby/Main/GoodsBarWidget.h"
#include "UI/Lobby/Equip/SetEffectWidget.h"
#include "UI/DataTable/SetEffectDataTable.h"
#include "DataAssets/UI/EquipUIDataAsset.h"
#include "Mode/Save/PGGameInstance.h"

void ULobbyEquipWidget::SetEquipList(EEquipCategory InType)
{
    CurrentActiveCategory = InType;
    EquipList->UpdateList(InType);

    SelectedEquip = nullptr;
    if (EquipButton)
    {
        EquipButton->SetIsEnabled(false);
    }
}

void ULobbyEquipWidget::NativeConstruct()
{
    GI = Cast<UPGGameInstance>(GetGameInstance());
    //GI->LoadGameData();
    if (GI)
    {
        GI->LoadGameData();
        GI->OnGoodsChanged.AddDynamic(this, &ULobbyEquipWidget::UpdateGoodsBar);

        if (Unlock) Unlock->InitializeGoodsBar(GI->CurrentPlayerUnlock);
        if (Gold) Gold->InitializeGoodsBar(GI->CurrentPlayerGold);
    }
    UnlockCostWidget->SetVisibility(ESlateVisibility::Hidden);
    UnlockCostWidget->InitializeGoodsBar(0);

    if (ExitButton) ExitButton->OnClicked.AddDynamic(this, &ULobbyEquipWidget::OnExitButtonClick); 
    if (WeaponEquip) WeaponEquip->OnSelected.AddDynamic(this, &ULobbyEquipWidget::SetEquipList);
    if (ArmorEquip) ArmorEquip->OnSelected.AddDynamic(this, &ULobbyEquipWidget::SetEquipList);
    if (AccesoryEquip) AccesoryEquip->OnSelected.AddDynamic(this, &ULobbyEquipWidget::SetEquipList);
    if (EquipList) EquipList->OnEquipSelected.AddDynamic(this, &ULobbyEquipWidget::HandleEquipSelected);

    if (EquipButton)
    {
        EquipButton->SetIsEnabled(false);
        EquipButton->OnClicked.AddDynamic(this, &ULobbyEquipWidget::OnEquipButtonClicked);
    }

    if (HeroStatus)
    {
        HeroStatus->SetHeroStatus();
    }

    IntializeEquipSlots();
    UpdateSetEffects();
    EquipDescription->SetVisibility(ESlateVisibility::Hidden);
}

void ULobbyEquipWidget::IntializeEquipSlots()
{
    UE_LOG(LogTemp, Warning, TEXT("Current Weapon Path: %s"), *GI->CurrentWeapon.ToString());
    WeaponEquip->UpdateEquipSlot(GI->CurrentWeapon.LoadSynchronous());
    ArmorEquip->UpdateEquipSlot(GI->CurrentArmor.LoadSynchronous());
    AccesoryEquip->UpdateEquipSlot(GI->CurrentAccessory.LoadSynchronous());
}

void ULobbyEquipWidget::UpdateGoodsBar(EGoodsCategory InCategory, int32 InValue)
{
    switch (InCategory)
    {
    case EGoodsCategory::Unlock: Unlock->UpdateGoodsText(InValue); break;
    case EGoodsCategory::Gold: Gold->UpdateGoodsText(InValue); break;
    }
}

void ULobbyEquipWidget::UpdateSetEffects()
{
    if (!SetEffectDataTable || !GI) return;

    // 현재 장착 중인 모든 아이템의 세트 태그 수집
    TMap<FGameplayTag, int32> EquippedTagCounts;

    auto CheckAndAddTag = [&](TSoftObjectPtr<UEquipUIDataAsset> AssetPtr) {
        if (auto Asset = AssetPtr.LoadSynchronous()) {
            if (Asset->SetTag.IsValid()) {
                EquippedTagCounts.FindOrAdd(Asset->SetTag)++;
            }
        }
        };

    CheckAndAddTag(GI->CurrentWeapon);
    CheckAndAddTag(GI->CurrentArmor);
    CheckAndAddTag(GI->CurrentAccessory);

    // 데이터 테이블을 순회하며 조건 검사
    bool bAnySetFound = false;
    static const FString ContextString(TEXT("SetEffectLookup"));

    TArray<FName> RowNames = SetEffectDataTable->GetRowNames();
    for (const FName& RowName : RowNames)
    {
        FSetEffectDataTable* Row = SetEffectDataTable->FindRow<FSetEffectDataTable>(RowName, ContextString);
        if (Row && EquippedTagCounts.Contains(Row->SetTag))
        {
            int32 CurrentCount = EquippedTagCounts[Row->SetTag];
            if (CurrentCount >= Row->RequiredCount)
            {
                // UI 업데이트
                SetEffectWidget->UpdateSetDisplay(Row->SetIcon, Row->SetName, Row->SetDescription);
                SetEffectWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

                bAnySetFound = true;
                break;
            }
        }
    }

    if (!bAnySetFound)
    {
        SetEffectWidget->SetVisibility(ESlateVisibility::Collapsed);
    }
}

void ULobbyEquipWidget::OnExitButtonClick()
{
    EquipList->ClearTileView();
    SelectedEquip = nullptr;
    EquipButton->SetIsEnabled(false);
    EquipDescription->SetVisibility(ESlateVisibility::Hidden); 
    IntializeEquipSlots();
    if (WidgetSwitcher) WidgetSwitcher->SetActiveWidgetIndex(0);
}

void ULobbyEquipWidget::OnEquipButtonClicked()
{
    if (!SelectedEquip) return;
    int32 TargetID = SelectedEquip->EquipID;

    if (GI->GetEquipOwned(TargetID))
    {
        // 현재 선택 카테고리에 따라 해당 부위 위젯 업데이트 및 인스턴스 저장
        switch (CurrentActiveCategory)
        {
        case EEquipCategory::Weapon:
            if (WeaponEquip) WeaponEquip->UpdateEquipSlot(SelectedEquip);
            GI->CurrentWeapon = SelectedEquip;
            break;
        case EEquipCategory::Armor:
            if (ArmorEquip) ArmorEquip->UpdateEquipSlot(SelectedEquip);
            GI->CurrentArmor = SelectedEquip;
            break;
        case EEquipCategory::Accessory:
            if (AccesoryEquip) AccesoryEquip->UpdateEquipSlot(SelectedEquip);
            GI->CurrentAccessory = SelectedEquip;
            break;
        }
    }
    else
    {
        if (GI->CurrentPlayerUnlock >= SelectedEquip->UnlockCost)
        {
            GI->ConsumeGoods(EGoodsCategory::Unlock, SelectedEquip->UnlockCost);

            // EquipMap 업데이트 및 저장
            GI->EquipMap.Add(TargetID, true);
            GI->SaveGameData();

            if (EquipButtonText) EquipButtonText->SetText(FText::FromString(TEXT("장착")));

            // 리스트 갱신 함수
            SetEquipList(CurrentActiveCategory);
            return;
        }
        else
        {
            // 재화 부족
            return;
        }
    }

    // 장착 후 처리
    UpdateSetEffects();
    GI->SaveGameData();
    EquipButton->SetIsEnabled(false);
    SelectedEquip = nullptr;
    EquipDescription->SetVisibility(ESlateVisibility::Hidden);
}

void ULobbyEquipWidget::HandleEquipSelected(UEquipUIDataAsset* InData)
{
    if(!InData) return;

    if (EquipDescription && InData)
    {
        // 정보 창 표시
        EquipDescription->SetVisibility(ESlateVisibility::Visible);
        EquipDescription->UpdateDescription(InData);
    }

    // 선택된 데이터 저장
    SelectedEquip = InData;

    // 장착 버튼 활성화
    if (EquipButton && EquipButtonText)
    {
        EquipButton->SetIsEnabled(true);

        bool bIsOwned = GI->GetEquipOwned(InData->EquipID);
        if (bIsOwned)
        {
            EquipButtonText->SetText(FText::FromString(TEXT("장착")));
            UnlockCostWidget->SetVisibility(ESlateVisibility::Hidden);
        }
        else
        {
            EquipButtonText->SetText(FText::FromString(TEXT("해금")));
            UnlockCostWidget->UpdateGoodsText(InData->UnlockCost);
            UnlockCostWidget->SetVisibility(ESlateVisibility::Visible);
            if (InData->UnlockCost >= GI->CurrentPlayerUnlock)
                EquipButton->SetIsEnabled(false);
            else
                EquipButton->SetIsEnabled(true);
        }
    }
}
