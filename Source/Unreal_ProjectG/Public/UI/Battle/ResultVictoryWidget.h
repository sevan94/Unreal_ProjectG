// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Types/PGEnumTypes.h"
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
    void ShowResult(const FBattleResultData& ResultData);

protected:
    virtual void NativeConstruct() override;

    UFUNCTION()
    void OnOpenStageClicked();

    UFUNCTION()
    void OnStageButtonClicked();
    
private:
    // 달성도 문구 수정
    FText GetRewardText(ERewardCategory Category, float RewardValue);

protected:
    // 다음 스테이지 버튼
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> OpenStage;

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

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> Star2Text;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> Star3Text;

    // 보상 텍스트
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> RewardGem;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> RewardGold;

    // 별 이미지
    UPROPERTY(BlueprintReadWrite, Category = "BattleResult")
    TObjectPtr<UTexture2D> StarImage;
};
