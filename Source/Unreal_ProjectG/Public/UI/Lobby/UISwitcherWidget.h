// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UISwitcherWidget.generated.h"

class UWidgetSwitcher;

/**
 * 
 */
UCLASS()
class UNREAL_PROJECTG_API UUISwitcherWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
    virtual void NativeConstruct() override;
	
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UWidgetSwitcher> WidgetSwitcher;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UUserWidget> LobbyWidget;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UUserWidget> UnitWidget;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UUserWidget> EquipWidget;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UUserWidget> GachaWidget;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UUserWidget> StageWidget;
};
