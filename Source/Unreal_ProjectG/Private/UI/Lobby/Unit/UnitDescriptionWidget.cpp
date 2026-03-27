// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Lobby/Unit/UnitDescriptionWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "DataAssets/UI/UnitUIDataAsset.h"
#include "UI/UnitEntryObject.h"
#include "UI/Lobby/Main/GoodsBarWidget.h"
#include "Mode/Save/PGGameInstance.h"
#include "Types/PGEnumTypes.h"
#include "Character/Unit/UnitCharacter.h"

void UUnitDescriptionWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (UpgradeButton)
    {
        UpgradeButton->OnClicked.AddDynamic(this, &UUnitDescriptionWidget::OnUpgradeButtonClicked);
    }
}

void UUnitDescriptionWidget::UpdateDescription(UUnitEntryObject* InEntryObject)
{
    if (!InEntryObject || !StatusCurveTable) return;
    CurrentEntryObject = InEntryObject;
    CurrentUIData = CurrentEntryObject->GetUnitUIData();

    UPGGameInstance* GI = Cast<UPGGameInstance>(GetGameInstance());
    if (!GI) return;

    // 저장된 데이터 가져오기
    SaveData = GI->GetUnitSaveData(CurrentUIData->UnitID);

    // 유닛 기본 정보 설정
    UnitName->SetText(FText::FromName(CurrentUIData->UnitName));
    UnitImage->SetBrushFromTexture(CurrentUIData->UnitImage);
    UnitCost->SetText(FText::AsNumber(CurrentUIData->UnitCost));
    UnitLevel->SetText(FText::AsNumber(SaveData.Level));
    UnitRank->SetBrushFromTexture(CurrentUIData->UnitRankImage);
    UnitType->SetBrushFromTexture(CurrentUIData->UnitTypeImage);

    int32 CurrentCost = 0;

    // 해금 여부에 따라 UI 갱신
    if (ButtonText && CostBar)
    {
        if (SaveData.bIsUnlocked)
        {
            ButtonText->SetText(FText::FromString(TEXT("강화")));
            CostBar->SetGoodsImage(GoldIcon);
            if (CurrentUIData->UnitStatus)
            {
                if (FRealCurve* CostCurve = CurrentUIData->UnitStatus->FindCurve(TEXT("UpgradeCost"), TEXT("")))
                {
                    CurrentCost = FMath::RoundToInt(CostCurve->Eval(SaveData.Level));
                    if (CurrentCost >= GI->CurrentPlayerGold)
                        UpgradeButton->SetIsEnabled(false);
                    else
                        UpgradeButton->SetIsEnabled(true);
                }
            }
        }
        else
        {
            ButtonText->SetText(FText::FromString(TEXT("해금")));
            CostBar->SetGoodsImage(UnlockIcon);
            CurrentCost = CurrentUIData->UnitUnlock;
            if (CurrentCost >= GI->CurrentPlayerUnlock)
                UpgradeButton->SetIsEnabled(false);
            else
                UpgradeButton->SetIsEnabled(true);
        }
        CostBar->UpdateGoodsText(CurrentCost);
    }

    SetUnitStatus();
}

void UUnitDescriptionWidget::OnUpgradeButtonClicked()
{
    if (!CurrentUIData || !CurrentEntryObject) return;

    UPGGameInstance* GI = Cast<UPGGameInstance>(GetGameInstance());
    if (!GI) return;

    // GameInstance의 맵에서 직접 데이터 참조를 가져와 수정
    if (FUnitSaveData* TargetData = GI->UnitLevelMap.Find(CurrentUIData->UnitID))
    {
        if (TargetData->bIsUnlocked)
        {
            // 강화 로직
            int32 CurrentCost = 0;
            if (CurrentUIData->UnitStatus)
            {
                if (FRealCurve* CostCurve = CurrentUIData->UnitStatus->FindCurve(TEXT("UpgradeCost"), TEXT("")))
                {
                    CurrentCost = FMath::RoundToInt(CostCurve->Eval(TargetData->Level));
                }
            }

            // 골드 체크
            if (GI->CurrentPlayerGold >= CurrentCost)
            {
                GI->ConsumeGoods(EGoodsCategory::Gold, CurrentCost);
                TargetData->Level++;
            }

        }
        else
        {
            // 해금 로직
            // 해금 재화 체크
            if (GI->CurrentPlayerUnlock >= CurrentUIData->UnitUnlock)
            {
                GI->ConsumeGoods(EGoodsCategory::Unlock, CurrentUIData->UnitUnlock);
                TargetData->bIsUnlocked = true;

                // 해금 성공 시 델리게이트 호출
                if (OnUnitUnlocked.IsBound())
                {
                    OnUnitUnlocked.Broadcast(CurrentEntryObject);
                }
            }
        }

        GI->SaveGameData();

        // UI 갱신
        UpdateDescription(CurrentEntryObject);

        // 타일뷰에 있는 리스트 아이템 위젯들에게 변경 알림
        CurrentEntryObject->BroadcastDataChanged();
    }
}

void UUnitDescriptionWidget::SetUnitStatus()
{
    if (!CurrentUIData || !CurrentUIData->UnitStatus) return;

    static const FString ContextString(TEXT("Unit Stat Context"));

    // 데이터 에셋에 들어있는 커브 테이블을 가져옴
    UCurveTable* TargetTable = CurrentUIData->UnitStatus;

    FRealCurve* HealthCurve = TargetTable->FindCurve(TEXT("MaxHealth"), ContextString);
    FRealCurve* AttackDamageCurve = TargetTable->FindCurve(TEXT("AttackDamage"), ContextString);
    FRealCurve* AttackSpeedCurve = TargetTable->FindCurve(TEXT("AttackSpeed"), ContextString);
    FRealCurve* MoveSpeedCurve = TargetTable->FindCurve(TEXT("MoveSpeed"), ContextString);

    // 데이터 적용
    if (HealthCurve && UnitHealth)
    {
        float HealthValue = HealthCurve->Eval(SaveData.Level);
        UnitHealth->SetText(FText::AsNumber(FMath::RoundToInt(HealthValue)));
    }
    if (AttackDamageCurve && UnitAttackDamage)
    {
        float AttackDamageValue = AttackDamageCurve->Eval(SaveData.Level);
        UnitAttackDamage->SetText(FText::AsNumber(FMath::RoundToInt(AttackDamageValue)));
    }
    if (AttackSpeedCurve && UnitAttackSpeed)
    {
        float AttackSpeedValue = AttackSpeedCurve->Eval(SaveData.Level);
        UnitAttackSpeed->SetText(FText::AsNumber(AttackSpeedValue));
    }
    if (MoveSpeedCurve && UnitMoveSpeed)
    {
        float MoveSpeedValue = MoveSpeedCurve->Eval(SaveData.Level);
        UnitMoveSpeed->SetText(FText::AsNumber(FMath::RoundToInt(MoveSpeedValue)));
    }
}
