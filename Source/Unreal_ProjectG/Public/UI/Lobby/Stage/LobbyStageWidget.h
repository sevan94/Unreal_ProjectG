// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/DataTable/StageDataTable.h"
#include "LobbyStageWidget.generated.h"

class UButton;
class UWidgetSwitcher;
class UStageMapWidget;
class UStageReadyWidget;

/**
 * 
 */
UCLASS()
class UNREAL_PROJECTG_API ULobbyStageWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
    virtual void NativeConstruct() override;

    UFUNCTION()
    void OpenStageInfo(int32 StageCode);

private:
    UFUNCTION()
    void OnExitButtonClick();

public:
    UPROPERTY(BlueprintReadOnly, Category = "UI")
    TObjectPtr<UWidgetSwitcher> WidgetSwitcher;

protected:
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UStageMapWidget> StageMap;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UStageReadyWidget> StageReady;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> ExitButton;
};
