// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Battle/BattleUIWidget.h"
#include "UI/Battle/ResultVictoryWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Kismet/GameplayStatics.h"
#include "Mode/PGBaseGameMode.h"
#include "Character/Hero/HeroCharacter.h"

void UBattleUIWidget::NativeConstruct()
{
    Super::NativeConstruct();

    PGGameMode = Cast<APGBaseGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

    if (PlaySpeedButton)
    {
        PlaySpeedButton->OnClicked.AddDynamic(this, &UBattleUIWidget::OnSpeedButtonClicked);
    }
    if (AutoButton)
    {
        AutoButton->OnClicked.AddDynamic(this, &UBattleUIWidget::OnAutoButtonClicked);
    }

    // 초기 텍스트 설정
    if (PlaySpeedText) PlaySpeedText->SetText(FText::FromString(TEXT("1.0x")));
}

void UBattleUIWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    if (PGGameMode.IsValid())
    {
        float CurrentPlayTime = PGGameMode->GetCurrentPlayTime();

        int32 TotalSeconds = FMath::FloorToInt(CurrentPlayTime);
        int32 Minutes = TotalSeconds / 60;
        int32 Seconds = TotalSeconds % 60;

        if (PlayTimeText)
        {
            FString TimeString = FString::Printf(TEXT("%02d:%02d"), Minutes, Seconds);
            PlayTimeText->SetText(FText::FromString(TimeString));
        }
    }
}

void UBattleUIWidget::OnSpeedButtonClicked()
{
    // 인덱스 순환
    CurrentSpeedIndex = (CurrentSpeedIndex + 1) % SpeedValues.Num();

    // 게임 배속 변경
    UGameplayStatics::SetGlobalTimeDilation(GetWorld(), SpeedValues[CurrentSpeedIndex]);

    if (CurrentSpeedIndex != 0)
    {
        SpeedActiveEffect->SetVisibility(ESlateVisibility::HitTestInvisible);
    }
    else
    {
        SpeedActiveEffect->SetVisibility(ESlateVisibility::Hidden);
    }

    // UI 업데이트
    if (PlaySpeedText)
    {
        FString SpeedString = FString::Printf(TEXT("%.1fx"), SpeedValues[CurrentSpeedIndex]);
        PlaySpeedText->SetText(FText::FromString(SpeedString));
    }

    UE_LOG(LogTemp, Log, TEXT("Current Game Speed: %.1fx"), SpeedValues[CurrentSpeedIndex]);
}

void UBattleUIWidget::ShowGameResult(const FBattleResultData& ResultData)
{
    PlayAnimation(ControlPanelSlide, 0.0f, 1);
    if (ResultData.bIsVictory)
    {
        ResultVictory->ShowResult(ResultData);
        ResultVictory->SetVisibility(ESlateVisibility::Visible);
        PlayAnimation(ResultVictoryScale);
    }
    else
    {
        ResultFail->SetVisibility(ESlateVisibility::Visible);
        PlayAnimation(ResultFailScale);
    }
}

void UBattleUIWidget::OnAutoButtonClicked()
{
    AHeroCharacter* Hero = Cast<AHeroCharacter>(GetOwningPlayerPawn());
    bIsAuto = !bIsAuto;
    if (bIsAuto)
    {
        Hero->SetAuto(true);
        Hero->ChangeCombatMode(EHeroCombatMode::Auto);
        PlayAnimation(ControlPanelSlide, 0.0f, 1);
        AutoActiveEffect->SetVisibility(ESlateVisibility::HitTestInvisible);
    }
    else
    {
        Hero->SetAuto(false);
        Hero->ChangeCombatMode(EHeroCombatMode::Manual);
        PlayAnimation(ControlPanelSlide, 0.0f, 1, EUMGSequencePlayMode::Reverse);
        AutoActiveEffect->SetVisibility(ESlateVisibility::Hidden);
    }
}
