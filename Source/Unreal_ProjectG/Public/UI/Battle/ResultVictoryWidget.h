// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ResultVictoryWidget.generated.h"

class UImage;
class UButton;
class UTextBlock;
struct FBattleResultData;

/**
 * 
 */
UCLASS()
class UNREAL_PROJECTG_API UResultVictoryWidget : public UUserWidget
{
	GENERATED_BODY()

public:
    void ShowResult(FBattleResultData& ResultData);

protected:
    virtual void NativeConstruct() override;

    UFUNCTION()
    void OnNextStageClicked();

    UFUNCTION()
    void OnStageButtonClicked();

protected:
    // 다음 스테이지 버튼
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> NextStage;

    // 스테이지 선택 버튼
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> StageButton;

    // 전투 정보
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> TotalPlayTime;
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> RemainHeartPercent;
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> TotalSpentCost;

    // 보상 달성도
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UImage> Star1;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UImage> Star2;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UImage> Star3;

    // 별 이미지
    UPROPERTY(BlueprintReadWrite, Category = "BattleResult")
    TObjectPtr<UTexture2D> StarImage;
};
