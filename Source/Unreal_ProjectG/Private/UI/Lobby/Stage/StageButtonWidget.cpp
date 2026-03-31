// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Lobby/Stage/StageButtonWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/Overlay.h"
#include "Mode/Save/PGGameInstance.h"

void UStageButtonWidget::InitStageButton(int32 InStageCode, const FText& InStageName, int32 InStarCount)
{
    StageCode = InStageCode;

    if (StageNumber)
    {
        StageNumber->SetText(InStageName);
    }

    UPGGameInstance* GI = Cast<UPGGameInstance>(GetGameInstance());
    if (!GI) return;

    // 스테이지 해금 여부 판별
    bool bIsUnlocked = false;

    // 첫 번째 스테이지는 항상 해금
    if (StageCode == 1)
    {
        bIsUnlocked = true;
    }
    // 이전 스테이지의 클리어 기록이 존재하는 경우
    else
    {
        int32 PreviousStageCode = StageCode - 1;
        if (GI->StageClearData.Contains(PreviousStageCode))
        {
            bIsUnlocked = true;
        }
    }

    switch (InStarCount)
    {
    case(3):Star3->SetBrushFromTexture(StarIcon);
    case(2):Star2->SetBrushFromTexture(StarIcon);
    case(1):Star1->SetBrushFromTexture(StarIcon);
    case(0):
    {
        if(bIsUnlocked)
            LockOverlay->SetVisibility(ESlateVisibility::Hidden);
        else
            LockOverlay->SetVisibility(ESlateVisibility::Visible);

    }
    default: break;
    }
}

void UStageButtonWidget::OnStageButtonClicked()
{
    if (OnStageSelected.IsBound())
    {
        OnStageSelected.Broadcast(StageCode);
    }
}

void UStageButtonWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (StageButton)
    {
        StageButton->OnClicked.AddDynamic(this, &UStageButtonWidget::OnStageButtonClicked);
    }
}