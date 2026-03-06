// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BarWidget.generated.h"

class UTextBlock;
class UImage;
class UProgressBar;

/**
 * 
 */
UCLASS()
class UNREAL_PROJECTG_API UBarWidget : public UUserWidget
{
	GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable)
    void InitProgressBar(UTexture2D* Icon, FLinearColor InColor, FText InName);

    void UpdateCurrent(float InCurrent);
    void UpdateMax(float InMax);
	
protected:
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UProgressBar> StatusBar = nullptr;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UImage> StatusIcon = nullptr;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> StatusName = nullptr;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> Current = nullptr;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> Max = nullptr;

private:
    float MaxValue = 1.0f;
};
