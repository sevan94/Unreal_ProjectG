// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Battle/BattleUIWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

void UBattleUIWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (PlaySpeedButton && AutoButton)
    {
        PlaySpeedButton->OnClicked.AddDynamic(this, &UBattleUIWidget::OnSpeedButtonClicked);
        AutoButton->OnClicked.AddDynamic(this, &UBattleUIWidget::OnAutoButtonClicked);
    }

    // 초기 텍스트 설정
    if (PlaySpeedText) PlaySpeedText->SetText(FText::FromString(TEXT("1.0x")));
}

void UBattleUIWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    // 배속 수치를 가져옴
    float CurrentDilatedTime = UGameplayStatics::GetGlobalTimeDilation(GetWorld());

    // 플레이 타임 누적
    ElapsedPlayTime += InDeltaTime * CurrentDilatedTime;

    int32 TotalSeconds = FMath::FloorToInt(ElapsedPlayTime);
    int32 Minutes = TotalSeconds / 60;
    int32 Seconds = TotalSeconds % 60;

    if (PlayTimeText)
    {
        FString TimeString = FString::Printf(TEXT("%02d:%02d"), Minutes, Seconds);
        PlayTimeText->SetText(FText::FromString(TimeString));
    }
}

void UBattleUIWidget::OnSpeedButtonClicked()
{
    // 인덱스 순환
    CurrentSpeedIndex = (CurrentSpeedIndex + 1) % SpeedValues.Num();

    // 게임 배속 변경
    UGameplayStatics::SetGlobalTimeDilation(GetWorld(), SpeedValues[CurrentSpeedIndex]);

    // UI 업데이트
    if (PlaySpeedText)
    {
        FString SpeedString = FString::Printf(TEXT("%.1fx"), SpeedValues[CurrentSpeedIndex]);
        PlaySpeedText->SetText(FText::FromString(SpeedString));
    }

    UE_LOG(LogTemp, Log, TEXT("Current Game Speed: %.1fx"), SpeedValues[CurrentSpeedIndex]);
}

void UBattleUIWidget::OnAutoButtonClicked()
{
    bIsAuto = !bIsAuto;
    if (bIsAuto)
    {
        PlayAnimation(ControlPanelMoveAnim, 0.0f, 1, EUMGSequencePlayMode::Forward);
    }
    else
    {
        PlayAnimation(ControlPanelMoveAnim, 0.0f, 1, EUMGSequencePlayMode::Reverse);
    }
}
