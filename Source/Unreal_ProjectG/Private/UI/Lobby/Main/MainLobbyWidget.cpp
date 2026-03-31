// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Lobby/Main/MainLobbyWidget.h"
#include "Components/WidgetSwitcher.h"
#include "Components/Button.h"
#include "UI/Lobby/Main/GoodsBarWidget.h"
#include "UI/Lobby/Main/OptionWidget.h"
#include "Mode/Save/PGGameInstance.h"

void UMainLobbyWidget::NativeConstruct()
{
    // 버튼 클릭 이벤트 델리게이트 바인딩
    if (StageButton) StageButton->OnClicked.AddDynamic(this, &UMainLobbyWidget::OnStageButtonClick);
    if (UnitButton)  UnitButton->OnClicked.AddDynamic(this, &UMainLobbyWidget::OnUnitButtonClick);
    if (EquipButton) EquipButton->OnClicked.AddDynamic(this, &UMainLobbyWidget::OnEquipButtonClick);
    if (GachaButton) GachaButton->OnClicked.AddDynamic(this, &UMainLobbyWidget::OnGachaButtonClick);
    if (OptionButton) OptionButton->OnClicked.AddDynamic(this, &UMainLobbyWidget::OnOptionButtonClick);
    if (OptionPanel) OptionPanel->SetVisibility(ESlateVisibility::Hidden);

    UPGGameInstance* GI = Cast<UPGGameInstance>(GetGameInstance());
    if (GI) GI->OnGoodsChanged.AddDynamic(this, &UMainLobbyWidget::UpdateGoodsBar);

    InitializeMainWidget();
}

void UMainLobbyWidget::InitializeMainWidget()
{
    UPGGameInstance* GI = Cast<UPGGameInstance>(GetGameInstance());
    
    Gem->InitializeGoodsBar(GI->CurrentPlayerGem);
    Unlock->InitializeGoodsBar(GI->CurrentPlayerUnlock);
    Gold->InitializeGoodsBar(GI->CurrentPlayerGold);
}

void UMainLobbyWidget::UpdateGoodsBar(EGoodsCategory InCategory, int32 InValue)
{
    switch (InCategory)
    {
    case EGoodsCategory::Gem: Gem->UpdateGoodsText(InValue); break;
    case EGoodsCategory::Unlock: Unlock->UpdateGoodsText(InValue); break;
    case EGoodsCategory::Gold: Gold->UpdateGoodsText(InValue); break;
    }
}

void UMainLobbyWidget::OnUnitButtonClick()
{
    if (WidgetSwitcher) WidgetSwitcher->SetActiveWidgetIndex(1);
}

void UMainLobbyWidget::OnEquipButtonClick()
{
    if (WidgetSwitcher) WidgetSwitcher->SetActiveWidgetIndex(2);
}

void UMainLobbyWidget::OnStageButtonClick()
{
    if (WidgetSwitcher) WidgetSwitcher->SetActiveWidgetIndex(3);
}

void UMainLobbyWidget::OnGachaButtonClick()
{
    if (WidgetSwitcher) WidgetSwitcher->SetActiveWidgetIndex(4);
}

void UMainLobbyWidget::OnOptionButtonClick()
{
    if (OptionPanel)
    {
        OptionPanel->SetVisibility(ESlateVisibility::Visible);
    }
}

