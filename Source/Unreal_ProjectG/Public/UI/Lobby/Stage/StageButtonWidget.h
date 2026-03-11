// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StageButtonWidget.generated.h"

class UImage;
class UButton;
class UTextBlock;

/**
 * 
 */
UCLASS()
class UNREAL_PROJECTG_API UStageButtonWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
    // 스테이지 데이터를 주입받아 위젯을 초기화하는 함수
    void InitStageButton(const FText& InStageName, int32 InStarCount);

protected:
    // 버튼 클릭 시 호출될 함수
    UFUNCTION()
    void OnStageButtonClicked();

    virtual void NativeConstruct() override;

protected:
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> StageNumber;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> StageButton;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UImage> Star1;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UImage> Star2;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UImage> Star3;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "UI")
    TObjectPtr<UTexture2D> StarIcon;
};
