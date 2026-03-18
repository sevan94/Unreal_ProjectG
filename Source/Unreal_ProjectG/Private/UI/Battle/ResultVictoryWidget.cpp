// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Battle/ResultVictoryWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Mode/PGBaseGameMode.h"
#include "Mode/Save/PGGameInstance.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "UI/DataTable/StageDataTable.h"

void UResultVictoryWidget::ShowResult(const FBattleResultData& ResultData)
{
    UPGGameInstance* GI = Cast<UPGGameInstance>(GetGameInstance());
    if (!GI) return;

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

    // 스테이지 정보를 가져옴
    const FStageDataTable& Data = GI->CurrentStageData;
    if (Star2Text) Star2Text->SetText(GetRewardText(Data.RewardType, Data.Star2));
    if (Star3Text) Star3Text->SetText(GetRewardText(Data.RewardType, Data.Star3));

    // 달성도 표시 로직
    if (StarImage)
    {
        switch (ResultData.StarCount)
        {
        case 3: if (Star3)
                Star3->SetBrushFromTexture(StarImage);
        case 2: if (Star2)
                Star2->SetBrushFromTexture(StarImage);
        case 1: if (Star1)
                Star1->SetBrushFromTexture(StarImage);
            break;
        default:
            break;
        }
    }

    int32* OldStars = GI->StageClearData.Find(Data.StageCode);
    bool bFirstTime = (OldStars == nullptr || *OldStars == 0);
    int32 DisplayGem = bFirstTime ? Data.RewardGem : 0;

    if (RewardGem) RewardGem->SetText(FText::AsNumber(DisplayGem));
    if (RewardGold) RewardGold->SetText(FText::AsNumber(Data.RewardGold * ResultData.StarCount));
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

FText UResultVictoryWidget::GetRewardText(ERewardCategory Category, float RewardValue)
{
    switch (Category)
    {
    case ERewardCategory::Time:
        return FText::FromString(FString::Printf(TEXT("%.0f초 이내로 클리어"), RewardValue));
    case ERewardCategory::Health:
        return FText::FromString(FString::Printf(TEXT("아군 기지 체력 %.0f%% 이상"), RewardValue));
    case ERewardCategory::Cost:
        return FText::FromString(FString::Printf(TEXT("소모 코스트 %.0f 이내 클리어"), RewardValue));
    default:
        return FText::FromString(TEXT("조건 정보 없음"));
    }
}
