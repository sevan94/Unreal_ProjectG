// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BattleUIWidget.generated.h"

class UTextBlock;
class UButton;
class UImage;
class UControlPanelWidget;
class UWidgetAnimation;

/**
 * 
 */
UCLASS()
class UNREAL_PROJECTG_API UBattleUIWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
    virtual void NativeConstruct() override;

    virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

    // 버튼 클릭 이벤트
    UFUNCTION()
    void OnSpeedButtonClicked();

    UFUNCTION()
    void OnAutoButtonClicked();
	
protected:
    // 플레이타임 텍스트
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> PlayTimeText;

    // 자동 버튼
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> AutoButton;
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UImage> AutoActiveEffect;

    // 배속 버튼
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> PlaySpeedButton;
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> PlaySpeedText;
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UImage> SpeedActiveEffect;

    // 일시 정지 버튼
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> PauseButton;

    // 컨트롤 패널 위젯
    UPROPERTY(BlueprintReadOnly,VisibleAnywhere, meta = (BindWidget))
    TObjectPtr<UControlPanelWidget> ControlPanel;

    UPROPERTY(Transient, meta = (BindWidgetAnimation))
    UWidgetAnimation* ControlPanelMoveAnim2;

private:
    // 현재 배속 단계 (0: 1배, 1: 1.5배, 2: 2배)
    int32 CurrentSpeedIndex = 0;

    // 배속 값 배열
    const TArray<float> SpeedValues = { 1.0f, 1.5f, 2.0f };

    // 플레이타임
    float ElapsedPlayTime = 0.0f;

    // 자동 모드 확인 변수
    bool bIsAuto = false;
};
