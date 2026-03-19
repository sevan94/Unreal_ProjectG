// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Lobby/Gacha/LobbyGachaWidget.h"
#include "Components/WidgetSwitcher.h"
#include "Components/Button.h"
#include "Mode/Save/PGGameInstance.h"
#include "UI/Lobby/Main/GoodsBarWidget.h"

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
    if (ExitButton) ExitButton->OnClicked.AddDynamic(this, &ULobbyGachaWidget::OnExitButtonClick);
}

void ULobbyGachaWidget::OnExitButtonClick()
{
    if (WidgetSwitcher) WidgetSwitcher->SetActiveWidgetIndex(0);
}

void ULobbyGachaWidget::UpdateGoodsBar(EGoodsCategory InCategory, int32 InValue)
{
    switch (InCategory)
    {
    case EGoodsCategory::Gem: Gem->UpdateGoodsText(InValue); break;
    case EGoodsCategory::Unlock: Unlock->UpdateGoodsText(InValue); break;
    }
}