// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ReadyUnitWidget.generated.h"

class UImage;
class UTextBlock;
class UUnitUIDataAsset;

/**
 * 
 */
UCLASS()
class UNREAL_PROJECTG_API UReadyUnitWidget : public UUserWidget
{
	GENERATED_BODY()

public:
    // UI 업데이트 함수
    void UpdateUnitWidget(TSoftObjectPtr<UUnitUIDataAsset> InData, int32 Level);
	
protected:
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UImage> UnitImage;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> UnitLevel;
};
