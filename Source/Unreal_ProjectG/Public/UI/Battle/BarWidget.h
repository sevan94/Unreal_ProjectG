// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BarWidget.generated.h"

class UTextBlock;
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
    void InitProgressBar(FLinearColor InColor, FText InName, float InMax);

    void UpdateCurrent(float InCurrent);
	
protected:
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UProgressBar> StatusBar = nullptr;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> StatusName = nullptr;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> Current = nullptr;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> Max = nullptr;

private:
    float MaxValue = 1.0f;
    float CurrentValue = 1.0f;
};
