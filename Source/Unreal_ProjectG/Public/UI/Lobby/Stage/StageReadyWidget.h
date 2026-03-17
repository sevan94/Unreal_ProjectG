// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StageReadyWidget.generated.h"

class UButton;
class UReadyUnitWidget;
class UPGGameInstance;

/**
 * 
 */
UCLASS()
class UNREAL_PROJECTG_API UStageReadyWidget : public UUserWidget
{
	GENERATED_BODY()

public:
    void InitializeReadyWidget();
	
protected:
    virtual void NativeConstruct() override;

    UFUNCTION()
    void OnExitButtonClicked();

protected:
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> ExitButton;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UReadyUnitWidget> ReadyUnit1;
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UReadyUnitWidget> ReadyUnit2;
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UReadyUnitWidget> ReadyUnit3;
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UReadyUnitWidget> ReadyUnit4;
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UReadyUnitWidget> ReadyUnit5;

private:
    TObjectPtr<UPGGameInstance> GI;
};
