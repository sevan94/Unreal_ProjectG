// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Types/PGEnumTypes.h"
#include "MainLobbyWidget.generated.h"

class UWidgetSwitcher;
class UButton;
class UGoodsBarWidget;
class UOptionWidget;

/**
 * 
 */
UCLASS()
class UNREAL_PROJECTG_API UMainLobbyWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
    virtual void NativeConstruct() override;

    void InitializeMainWidget();

    UFUNCTION()
    void UpdateGoodsBar(EGoodsCategory InCategory, int32 InValue);

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

    UFUNCTION()
    void OnOptionButtonClick();

public:
    // 위젯 스위처
    UPROPERTY(BlueprintReadOnly, Category = "UI")
    TObjectPtr<UWidgetSwitcher> WidgetSwitcher;

protected:
    // 버튼 바인딩
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> EquipButton;
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> UnitButton;
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> StageButton;
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> GachaButton;
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> OptionButton;

    // 재화 창
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UGoodsBarWidget> Gem;
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UGoodsBarWidget> Unlock;
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UGoodsBarWidget> Gold;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UOptionWidget> OptionPanel;
};
