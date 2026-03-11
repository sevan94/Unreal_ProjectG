// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Lobby/Stage/StageButtonWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/Image.h"

void UStageButtonWidget::InitStageButton(const FText& InStageName, int32 InStarCount)
{
    if (StageNumber)
    {
        StageNumber->SetText(InStageName);
    }

    switch (InStarCount)
    {
    case(3):Star3->SetBrushFromTexture(StarIcon);
    case(2):Star2->SetBrushFromTexture(StarIcon);
    case(1):Star1->SetBrushFromTexture(StarIcon);
    default: break;
    }
}

void UStageButtonWidget::OnStageButtonClicked()
{
    // 스테이지 정보 패널 출력
}

void UStageButtonWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (StageButton)
    {
        StageButton->OnClicked.AddDynamic(this, &UStageButtonWidget::OnStageButtonClicked);
    }
}