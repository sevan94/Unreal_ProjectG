// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Lobby/UnitDescriptionWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "DataAssets/UI/UnitUIDataAsset.h"

void UUnitDescriptionWidget::UpdateDescription(UUnitUIDataAsset* InData)
{
    if (!InData) return;

    // 이름 설정
    UnitName->SetText(FText::FromName(InData->UnitName));

    // 이미지 설정
    UnitImage->SetBrushFromTexture(InData->UnitImage);

    // 2. GE/커브 테이블 수치 읽기
    if (StatusCurveTable)
    {
        // 행 이름 예: "PioneerMaxHealth"
        static const FString ContextString(TEXT("Unit Stat Context"));

        // 실제 프로젝트에서는 InData 내부의 RowName 변수를 사용하는 것이 좋습니다.
        FRealCurve* HealthCurve = StatusCurveTable->FindCurve(TEXT("PioneerMaxHealth"), ContextString);
        FRealCurve* AttackPowerCurve = StatusCurveTable->FindCurve(TEXT("PioneerAttackPower"), ContextString);
        if (HealthCurve && AttackPowerCurve)
        {
            float HealthValue = HealthCurve->Eval(1.0f);    // 임시 레벨 값
            UnitHealth->SetText(FText::AsNumber(HealthValue));
        }
    }
}
