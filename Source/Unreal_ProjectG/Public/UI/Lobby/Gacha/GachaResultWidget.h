// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DataAssets/UI/UnitUIDataAsset.h"
#include "GachaResultWidget.generated.h"

class UButton;
class UImage;
class UNiagaraSystemWidget;
class UWidgetSwitcher;

/**
 * 
 */
UCLASS()
class UNREAL_PROJECTG_API UGachaResultWidget : public UUserWidget
{
	GENERATED_BODY()

public:
    void InitData(UUnitUIDataAsset* InData);

    void PlayGachaAnim();

protected:
    virtual void NativeConstruct() override;

    UFUNCTION()
    void OnSkipButtonClicked();

public:
    UPROPERTY(BlueprintReadOnly, Category = "UI")
    TObjectPtr<UWidgetSwitcher> WidgetSwitcher;
	
protected:
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> SkipButton;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UImage> UnitImage;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UNiagaraSystemWidget> GachaEffect;

    UPROPERTY(BlueprintReadWrite, Transient, meta = (BindWidgetAnim))
    UWidgetAnimation* UnitGacha;

private:
    TObjectPtr<UUnitUIDataAsset> PickupUnit;
};
