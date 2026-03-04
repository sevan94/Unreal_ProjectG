// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Battle/ResultVictoryWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Mode/PGBaseGameMode.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UResultVictoryWidget::ShowResult(const FBattleResultData& ResultData)
{
    // 플레이 타임 표시
    int32 Minutes = FMath::FloorToInt(ResultData.TotalPlayTime / 60.0f);
    int32 Seconds = FMath::FloorToInt(ResultData.TotalPlayTime) % 60;
    FString TimeString = FString::Printf(TEXT("%02d:%02d"), Minutes, Seconds);

    if (TotalPlayTime)
    {
        TotalPlayTime->SetText(FText::FromString(TimeString));
    }

    // 남은 체력 표시
    if (RemainHeartPercent)
    {
        FString HealthString = FString::Printf(TEXT("%d%%"), FMath::RoundToInt(ResultData.RemainingHealthPercent));
        RemainHeartPercent->SetText(FText::FromString(HealthString));
    }

    // 총 소모 코스트 표시
    if (TotalSpentCost)
    {
        TotalSpentCost->SetText(FText::AsNumber(ResultData.TotalSpentCost));
    }

    // 달성도 표시 로직
    if (StarImage)
    {
        switch (ResultData.StarCount)
        {
        case 3:
            if (Star3)
            {
                Star3->SetBrushFromTexture(StarImage);
            }
        case 2:
            if (Star2)
            {
                Star2->SetBrushFromTexture(StarImage);
            }
        case 1:
            if (Star1)
            {
                Star1->SetBrushFromTexture(StarImage);
            }
            break;
        default:
            break;
        }
    }
}

void UResultVictoryWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // 다음 스테이지 버튼 클릭 시
    if (NextStage)
    {
        NextStage->OnClicked.AddDynamic(this, &UResultVictoryWidget::OnNextStageClicked);
    }

    // 스테이지 선택(로비) 버튼 클릭 시
    if (StageButton)
    {
        StageButton->OnClicked.AddDynamic(this, &UResultVictoryWidget::OnStageButtonClicked);
    }
}

void UResultVictoryWidget::OnNextStageClicked()
{
    UGameplayStatics::OpenLevel(GetWorld(), TEXT("Stage"));
}

void UResultVictoryWidget::OnStageButtonClicked()
{
    UGameplayStatics::OpenLevel(GetWorld(), TEXT("Lobby"));
}
