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

    static const FString ContextString(TEXT("Unit Stat Context"));

    FName HealthRowName = *FString::Printf(TEXT("%sMaxHealth"), *InData->UnitStatus);
    FName AttackRowName = *FString::Printf(TEXT("%sAttackPower"), *InData->UnitStatus);

    // 2. 커브 찾기
    FRealCurve* HealthCurve = StatusCurveTable->FindCurve(HealthRowName, ContextString);
    FRealCurve* AttackPowerCurve = StatusCurveTable->FindCurve(AttackRowName, ContextString);

    // 3. 데이터 적용
    if (HealthCurve)
    {
        float HealthValue = HealthCurve->Eval(1.0f);
        UnitHealth->SetText(FText::AsNumber(HealthValue));
    }

    if (AttackPowerCurve)
    {
        float AttackValue = AttackPowerCurve->Eval(1.0f);
        // UnitAttack->SetText(FText::AsNumber(AttackValue)); // UI에 맞게 추가
    }
}
