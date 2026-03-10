// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Lobby/Unit/UnitDescriptionWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "DataAssets/UI/UnitUIDataAsset.h"
#include "UI/UnitEntryObject.h"
#include "Mode/Save/PGGameInstance.h"

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

    // 해금 여부에 따라 버튼 텍스트 변경
    if (ButtonText)
    {
        FText ButtonLabel = SaveData.bIsUnlocked ? FText::FromString(TEXT("강화")) : FText::FromString(TEXT("해금"));
        ButtonText->SetText(ButtonLabel);
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
            // 골드 체크
            // if(GI->Gold >= UpgradeCost)

            // 레벨업 실행 후 저장
            TargetData->Level++;
        }
        else
        {
            // 해금 로직
            // 해금 재화 체크
            // if(GI->Unlock >= UnlockCost) 
            TargetData->bIsUnlocked = true;
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
    static const FString ContextString(TEXT("Unit Stat Context"));

    FName HealthRowName = *FString::Printf(TEXT("%sMaxHealth"), *CurrentUIData->UnitStatus);
    FName AttackDamageRowName = *FString::Printf(TEXT("%sAttackDamage"), *CurrentUIData->UnitStatus);
    FName AttackSpeedRowName = *FString::Printf(TEXT("%sAttackSpeed"), *CurrentUIData->UnitStatus);
    FName MoveSpeedRowName = *FString::Printf(TEXT("%sMoveSpeed"), *CurrentUIData->UnitStatus);

    // 커브 찾기
    FRealCurve* HealthCurve = StatusCurveTable->FindCurve(HealthRowName, ContextString);
    FRealCurve* AttackDamageCurve = StatusCurveTable->FindCurve(AttackDamageRowName, ContextString);
    FRealCurve* AttackSpeedCurve = StatusCurveTable->FindCurve(AttackSpeedRowName, ContextString);
    FRealCurve* MoveSpeedCurve = StatusCurveTable->FindCurve(MoveSpeedRowName, ContextString);

    // 데이터 적용
    if (HealthCurve)
    {
        float HealthValue = HealthCurve->Eval(SaveData.Level);
        UnitHealth->SetText(FText::AsNumber(HealthValue));
    }

    if (AttackDamageCurve)
    {
        float AttackDamageValue = AttackDamageCurve->Eval(SaveData.Level);
        UnitAttackDamage->SetText(FText::AsNumber(AttackDamageValue));
    }

    if (AttackSpeedCurve)
    {
        float AttackSpeedValue = AttackSpeedCurve->Eval(SaveData.Level);
        UnitAttackSpeed->SetText(FText::AsNumber(AttackSpeedValue));
    }

    if (MoveSpeedCurve)
    {
        float MoveSpeedValue = MoveSpeedCurve->Eval(SaveData.Level);
        UnitMoveSpeed->SetText(FText::AsNumber(MoveSpeedValue));
    }
}
