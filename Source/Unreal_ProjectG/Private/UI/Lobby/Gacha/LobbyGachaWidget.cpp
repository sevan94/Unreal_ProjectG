// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Lobby/Gacha/LobbyGachaWidget.h"
#include "Components/WidgetSwitcher.h"
#include "Components/Button.h"
#include "Mode/PGLobbyMode.h"
#include "Mode/Save/PGGameInstance.h"
#include "Mode/Save/PGUnitCollectionSubsystem.h"
#include "Mode/Save/PGEquipCollectionSubsystem.h"
#include "UI/Lobby/Main/GoodsBarWidget.h"
#include "UI/Lobby/Gacha/GachaResultWidget.h"
#include "UI/Lobby/Gacha/GachaActor.h"
#include "UI/Lobby/Gacha/EquipGachaActor.h"
#include "DataAssets/UI/UnitUIDataAsset.h"

void ULobbyGachaWidget::NativeConstruct()
{
    GI = GetGameInstance<UPGGameInstance>();
    
    if (GI)
    {
        GI->LoadGameData();
        GI->OnGoodsChanged.AddDynamic(this, &ULobbyGachaWidget::UpdateGoodsBar);

        Gem->InitializeGoodsBar(GI->CurrentPlayerGem);
        Unlock->InitializeGoodsBar(GI->CurrentPlayerUnlock);
    }

    if (UnitGachaCost) UnitGachaCost->InitializeGoodsBar(100);
    if (EquipGachaCost) EquipGachaCost->InitializeGoodsBar(100);
    if (UnitGachaButton) UnitGachaButton->OnClicked.AddDynamic(this, &ULobbyGachaWidget::OnUnitGachaClick);
    if (EquipGachaButton) EquipGachaButton->OnClicked.AddDynamic(this, &ULobbyGachaWidget::OnEquipGachaClick);
    if (ExitButton) ExitButton->OnClicked.AddDynamic(this, &ULobbyGachaWidget::OnExitButtonClick);
}

void ULobbyGachaWidget::OnExitButtonClick()
{
    if (WidgetSwitcher) WidgetSwitcher->SetActiveWidgetIndex(0);
}

void ULobbyGachaWidget::OnUnitGachaClick()
{
    if (GI->CurrentPlayerGem < 100) return;

    UPGUnitCollectionSubsystem* Subsystem = GI->GetSubsystem<UPGUnitCollectionSubsystem>();
    UUnitUIDataAsset* PickupUnit = Subsystem->RollSingleGacha();
    GI->ConsumeGoods(EGoodsCategory::Gem, 100);

    if (PickupUnit)
    {
        UGachaResultWidget* ResultWidget = Cast<UGachaResultWidget>(WidgetSwitcher->GetWidgetAtIndex(5));

        if (ResultWidget)
        {
            // 데이터 초기화
            ResultWidget->InitUnitData(PickupUnit);
        }
    }

    // 화면 전환
    if (WidgetSwitcher) WidgetSwitcher->SetActiveWidgetIndex(5);

    APGLobbyMode* GM = Cast<APGLobbyMode>(GetWorld()->GetAuthGameMode());
    if (GM && GM->UnitGachaActor)
    {
        GM->UnitGachaActor->GachaReset();
        GM->PlayUnitGacha(PickupUnit);
    }
}

void ULobbyGachaWidget::OnEquipGachaClick()
{
    if (GI->CurrentPlayerGem < 100) return;
    UPGEquipCollectionSubsystem* Subsystem = GI->GetSubsystem<UPGEquipCollectionSubsystem>();
    UEquipUIDataAsset* PickupEquip = Subsystem->RollSingleGacha();
    GI->ConsumeGoods(EGoodsCategory::Gem, 100);

    if (PickupEquip)
    {
        UGachaResultWidget* ResultWidget = Cast<UGachaResultWidget>(WidgetSwitcher->GetWidgetAtIndex(5));

        if (ResultWidget)
        {
            // 데이터 초기화
            ResultWidget->InitEquipData(PickupEquip);
        }
    }

    // 화면 전환
    if (WidgetSwitcher) WidgetSwitcher->SetActiveWidgetIndex(5);

    APGLobbyMode* GM = Cast<APGLobbyMode>(GetWorld()->GetAuthGameMode());
    if (GM && GM->EquipGachaActor)
    {
        GM->EquipGachaActor->ChestReset();
        GM->PlayEquipGacha();
    }
}

void ULobbyGachaWidget::UpdateGoodsBar(EGoodsCategory InCategory, int32 InValue)
{
    switch (InCategory)
    {
    case EGoodsCategory::Gem: Gem->UpdateGoodsText(InValue); break;
    case EGoodsCategory::Unlock: Unlock->UpdateGoodsText(InValue); break;
    }
}