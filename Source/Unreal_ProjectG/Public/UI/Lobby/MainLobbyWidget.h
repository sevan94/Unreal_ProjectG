// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainLobbyWidget.generated.h"

class UWidgetSwitcher;
class UButton;

/**
 * 
 */
UCLASS()
class UNREAL_PROJECTG_API UMainLobbyWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
    virtual void NativeConstruct() override;

private:
    // 각 버튼 클릭 시 실행될 함수들
    UFUNCTION()
    void OnUnitButtonClick();

    UFUNCTION()
    void OnEquipButtonClick();

    UFUNCTION()
    void OnStageButtonClick();

    UFUNCTION()
    void OnGachaButtonClick();

public:
    UPROPERTY(BlueprintReadOnly, Category = "UI")
    TObjectPtr<UWidgetSwitcher> WidgetSwitcher;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> EquipButton;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> UnitButton;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> StageButton;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> GachaButton;
};
