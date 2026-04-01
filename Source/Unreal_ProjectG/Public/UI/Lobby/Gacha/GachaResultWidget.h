// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DataAssets/UI/UnitUIDataAsset.h"
#include "DataAssets/UI/EquipUIDataAsset.h"
#include "GachaResultWidget.generated.h"

class UButton;
class UImage;
class UTextBlock;
class UCanvasPanel;
class UNiagaraSystemWidget;
class UWidgetSwitcher;
class UAudioComponent;

/**
 * 
 */
UCLASS()
class UNREAL_PROJECTG_API UGachaResultWidget : public UUserWidget
{
	GENERATED_BODY()

public:
    void InitUnitData(UUnitUIDataAsset* InData);
    void InitEquipData(UEquipUIDataAsset* InData);

    void PlayGachaAnim();
    void ShowEquipResult();

protected:
    virtual void NativeConstruct() override;

    virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

    UFUNCTION()
    void OnSkipButtonClicked();

    UFUNCTION()
    void OnGachaAnimFinished();

    void ResetGachaResult();

public:
    UPROPERTY(BlueprintReadOnly, Category = "UI")
    TObjectPtr<UWidgetSwitcher> WidgetSwitcher;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UCanvasPanel> GachaReaultPanel;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UCanvasPanel> UnitPanel;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UCanvasPanel> EquipPanel;

protected:
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> SkipButton;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UImage> UnitImage;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UImage> UnitRank;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> UnitName;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UImage> EquipImage;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UImage> EquipGachaTarget;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UNiagaraSystemWidget> GachaEffect;

    UPROPERTY(BlueprintReadWrite, Transient, meta = (BindWidgetAnim))
    UWidgetAnimation* UnitGacha;

    UPROPERTY()
    TObjectPtr<USoundBase> UnitSound;

    UPROPERTY()
    TObjectPtr<UAudioComponent> AudioComponent;

    bool bCanExit = false;
private:
    TObjectPtr<UUnitUIDataAsset> PickupUnit;
    TObjectPtr<UEquipUIDataAsset> PickupEquip;
};
