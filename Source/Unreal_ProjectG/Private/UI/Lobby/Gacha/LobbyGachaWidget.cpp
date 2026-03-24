// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Lobby/Gacha/LobbyGachaWidget.h"
#include "Components/WidgetSwitcher.h"
#include "Components/Button.h"
#include "Mode/PGLobbyMode.h"
#include "Mode/Save/PGGameInstance.h"
#include "Mode/Save/PGUnitCollectionSubsystem.h"
#include "UI/Lobby/Main/GoodsBarWidget.h"
#include "UI/Lobby/Gacha/GachaResultWidget.h"
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
    UPGUnitCollectionSubsystem* Subsystem = GI->GetSubsystem<UPGUnitCollectionSubsystem>();
    UUnitUIDataAsset* PickupUnit = Subsystem->RollSingleGacha();

    // 화면 전환
    if (WidgetSwitcher) WidgetSwitcher->SetActiveWidgetIndex(5);

    APGLobbyMode* GM = Cast<APGLobbyMode>(GetWorld()->GetAuthGameMode());
    if (GM && GM->GachaActor)
    {
        GM->PlayGacha();
    }

    if (PickupUnit)
    {
        UGachaResultWidget* ResultWidget = Cast<UGachaResultWidget>(WidgetSwitcher->GetWidgetAtIndex(5));

        if (ResultWidget)
        {
            // 데이터 초기화 및 애니메이션 재생
            ResultWidget->InitData(PickupUnit);
            ResultWidget->PlayGachaAnim();
        }
    }
}

void ULobbyGachaWidget::OnEquipGachaClick()
{
}

void ULobbyGachaWidget::UpdateGoodsBar(EGoodsCategory InCategory, int32 InValue)
{
    switch (InCategory)
    {
    case EGoodsCategory::Gem: Gem->UpdateGoodsText(InValue); break;
    case EGoodsCategory::Unlock: Unlock->UpdateGoodsText(InValue); break;
    }
}