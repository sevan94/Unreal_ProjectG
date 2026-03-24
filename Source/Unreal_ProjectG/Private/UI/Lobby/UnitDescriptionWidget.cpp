// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Lobby/UnitDescriptionWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "DataAssets/UI/UnitUIDataAsset.h"

void UUnitDescriptionWidget::UpdateDescription(UUnitUIDataAsset* InData)
{
    if (!InData || !StatusCurveTable) return;

    // 유닛 기본 정보 설정
    UnitName->SetText(FText::FromName(InData->UnitName));
    UnitImage->SetBrushFromTexture(InData->UnitImage);
    UnitCost->SetText(FText::AsNumber(InData->UnitCost));

    static const FString ContextString(TEXT("Unit Stat Context"));

    FName HealthRowName = *FString::Printf(TEXT("%sMaxHealth"), *InData->UnitStatus);
    FName AttackDamageRowName = *FString::Printf(TEXT("%sAttackDamage"), *InData->UnitStatus);
    FName AttackSpeedRowName = *FString::Printf(TEXT("%sAttackSpeed"), *InData->UnitStatus);
    FName MoveSpeedRowName = *FString::Printf(TEXT("%sMoveSpeed"), *InData->UnitStatus);

    // 커브 찾기
    FRealCurve* HealthCurve = StatusCurveTable->FindCurve(HealthRowName, ContextString);
    FRealCurve* AttackDamageCurve = StatusCurveTable->FindCurve(AttackDamageRowName, ContextString);
    FRealCurve* AttackSpeedCurve = StatusCurveTable->FindCurve(AttackSpeedRowName, ContextString);
    FRealCurve* MoveSpeedCurve = StatusCurveTable->FindCurve(MoveSpeedRowName, ContextString);

    // 데이터 적용
    if (HealthCurve)
    {
        float HealthValue = HealthCurve->Eval(1.0f);
        UnitHealth->SetText(FText::AsNumber(HealthValue));
    }

    if (AttackDamageCurve)
    {
        float AttackDamageValue = AttackDamageCurve->Eval(1.0f);
        UnitAttackDamage->SetText(FText::AsNumber(AttackDamageValue));
    }

    if (AttackSpeedCurve)
    {
        float AttackSpeedValue = AttackSpeedCurve->Eval(1.0f);
        UnitAttackSpeed->SetText(FText::AsNumber(AttackSpeedValue));
    }

    if (MoveSpeedCurve)
    {
        float MoveSpeedValue = MoveSpeedCurve->Eval(1.0f);
        UnitMoveSpeed->SetText(FText::AsNumber(MoveSpeedValue));
    }
}
