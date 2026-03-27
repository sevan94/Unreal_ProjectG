// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Lobby/Equip/HeroStatusWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "UI/Lobby/Main/GoodsBarWidget.h"
#include "Mode/Save/PGGameInstance.h"

void UHeroStatusWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (UpgradeButton)
    {
        UpgradeButton->OnClicked.AddDynamic(this, &UHeroStatusWidget::OnUpgradeButtonClicked);
    }

    GI = Cast<UPGGameInstance>(GetGameInstance());
    if (GI)
    {
        if (StatusCurveTable)
        {
            if (FRealCurve* CostCurve = StatusCurveTable->FindCurve(TEXT("UpgradeCost"), TEXT("")))
            {
                HeroUpgradeCost->InitializeGoodsBar(FMath::RoundToInt(CostCurve->Eval(GI->CurrentPlayerLevel)));
            }
        }
        SetHeroStatus();
    }
}

void UHeroStatusWidget::OnUpgradeButtonClicked()
{
    // 강화 로직
    int32 CurrentCost = 0;
    if (StatusCurveTable)
    {
        if (FRealCurve* CostCurve = StatusCurveTable->FindCurve(TEXT("UpgradeCost"), TEXT("")))
        {
            CurrentCost = FMath::RoundToInt(CostCurve->Eval(GI->CurrentPlayerLevel));
        }
    }
    // 골드 체크
    if (GI->CurrentPlayerGold >= CurrentCost)
    {
        GI->ConsumeGoods(EGoodsCategory::Gold, CurrentCost);
        GI->CurrentPlayerLevel++;
    }

    GI->SaveGameData();

    // UI 갱신
    SetHeroStatus();
}

void UHeroStatusWidget::SetHeroStatus()
{
    static const FString ContextString(TEXT("Hero Stat Context"));
    if (!StatusCurveTable) return;

    FRealCurve* HealthCurve = StatusCurveTable->FindCurve(TEXT("MaxHealth"), ContextString);
    FRealCurve* AttackPowerCurve = StatusCurveTable->FindCurve(TEXT("AttackPower"), ContextString);
    FRealCurve* AttackSpeedCurve = StatusCurveTable->FindCurve(TEXT("AttackSpeed"), ContextString);
    FRealCurve* MaxCostCurve = StatusCurveTable->FindCurve(TEXT("MaxCost"), ContextString);
    FRealCurve* CostRegenAmountCurve = StatusCurveTable->FindCurve(TEXT("CostRegenAmount"), ContextString);
    FRealCurve* CostRegenIntervalCurve = StatusCurveTable->FindCurve(TEXT("CostRegenInterval"), ContextString);
    FRealCurve* UpgradeCostCurve = StatusCurveTable->FindCurve(TEXT("UpgradeCost"), ContextString);

    // 데이터 적용
    if (HeroLevel)
    {
        HeroLevel->SetText(FText::AsNumber(GI->CurrentPlayerLevel));
    }
    if (HealthCurve && HeroHealth)
    {
        float HealthValue = HealthCurve->Eval(GI->CurrentPlayerLevel);
        HeroHealth->SetText(FText::AsNumber(FMath::RoundToInt(HealthValue)));
    }
    if (AttackPowerCurve && HeroAttackPower)
    {
        float AttackPowerValue = AttackPowerCurve->Eval(GI->CurrentPlayerLevel);
        HeroAttackPower->SetText(FText::AsNumber(FMath::RoundToInt(AttackPowerValue)));
    }
    if (AttackSpeedCurve && HeroAttackSpeed)
    {
        float AttackSpeedValue = AttackSpeedCurve->Eval(GI->CurrentPlayerLevel);
        HeroAttackSpeed->SetText(FText::AsNumber(AttackSpeedValue));
    }
    if (MaxCostCurve && HeroMaxCost)
    {
        float MaxCostValue = MaxCostCurve->Eval(GI->CurrentPlayerLevel);
        HeroMaxCost->SetText(FText::AsNumber(FMath::RoundToInt(MaxCostValue)));
    }
    if (CostRegenAmountCurve && HeroCostRegenAmount)
    {
        float CostRegenAmountValue = CostRegenAmountCurve->Eval(GI->CurrentPlayerLevel);
        HeroCostRegenAmount->SetText(FText::AsNumber(FMath::RoundToInt(CostRegenAmountValue)));
    }
    if (CostRegenIntervalCurve && HeroCostRegenInterval)
    {
        float CostRegenInterval = CostRegenIntervalCurve->Eval(GI->CurrentPlayerLevel);
        HeroCostRegenInterval->SetText(FText::AsNumber(FMath::RoundToInt(CostRegenInterval)));
    }
    if (UpgradeCostCurve && HeroUpgradeCost)
    {
        float UpgradeCost = UpgradeCostCurve->Eval(GI->CurrentPlayerLevel);
        HeroUpgradeCost->InitializeGoodsBar(UpgradeCost);
    }
}