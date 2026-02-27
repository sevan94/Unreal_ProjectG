// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LobbyUnitWidget.generated.h"

class UButton;
class UWidgetSwitcher;
class UUnitListWidget;
class UUnitDescriptionWidget; 
class UUnitUIDataAsset;

/**
 * 
 */
UCLASS()
class UNREAL_PROJECTG_API ULobbyUnitWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
    virtual void NativeConstruct() override;

private:
    UFUNCTION()
    void OnExitButtonClick();

    UFUNCTION()
    void HandleUnitSelected(UUnitUIDataAsset* SelectedData);

public:
    UPROPERTY(BlueprintReadOnly, Category = "UI")
    TObjectPtr<UWidgetSwitcher> WidgetSwitcher;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UUnitListWidget> UnitList;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UUnitDescriptionWidget> UnitDescription;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> ExitButton;
};
