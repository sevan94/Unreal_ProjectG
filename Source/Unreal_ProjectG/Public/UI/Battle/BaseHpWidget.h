// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BaseHpWidget.generated.h"

class UProgressBar;
class UTextBlock;

/**
 * 
 */
UCLASS()
class UNREAL_PROJECTG_API UBaseHpWidget : public UUserWidget
{
	GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable)
    void InitHPBar(float InCurrent, float InMax);

    void UpdateCurrentHP(float InCurrent);
	
protected:
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UProgressBar> HPBar = nullptr;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> CurrentHP = nullptr;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> MaxHP = nullptr;

private:
    float MaxValue = 1.0f;
    float CurrentValue = 1.0f;
};
