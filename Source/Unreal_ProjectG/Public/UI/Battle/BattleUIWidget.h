// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BattleUIWidget.generated.h"

class UTextBlock;
class UButton;
class UImage;
class UControlPanelWidget;
class UResultVictoryWidget;
class UPauseWidget;
class UWidgetAnimation;
class APGBaseGameMode;
struct FBattleResultData;

/**
 * 
 */
UCLASS()
class UNREAL_PROJECTG_API UBattleUIWidget : public UUserWidget
{
	GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable)
    void ShowGameResult(const FBattleResultData& ResultData);

    UControlPanelWidget* GetControlPanel() { return ControlPanel; }

protected:
    virtual void NativeConstruct() override;

    virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

    // 마우스 버튼 클릭 시
    virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
    // 마우스 버튼 드래그 시
    virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
    // 마우스 버튼 클릭 해제 시
    virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

    // 버튼 클릭 이벤트
    UFUNCTION()
    void OnSpeedButtonClicked();

    UFUNCTION()
    void OnAutoButtonClicked();

    UFUNCTION()
    void OnPauseButtonClicked();

    UFUNCTION()
    void AutoUnitSpawn();
	
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

    // 전투 승리 위젯
    UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (BindWidget))
    TObjectPtr<UResultVictoryWidget> ResultVictory;

    // 전투 패배 위젯
    UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (BindWidget))
    TObjectPtr<UUserWidget> ResultFail;

    // 일시정지 위젯
    UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (BindWidget))
    TObjectPtr<UPauseWidget> PauseWidget;

    // 게임 모드
    TWeakObjectPtr<APGBaseGameMode> PGGameMode;

    // 자동 모드 확인 변수
    UPROPERTY(BlueprintReadWrite)
    bool bIsAuto = false;

    // 위젯 애니메이션
    UPROPERTY(BlueprintReadWrite, Transient, meta = (BindWidgetAnim))
    UWidgetAnimation* ControlPanelSlide;

    UPROPERTY(BlueprintReadWrite, Transient, meta = (BindWidgetAnim))
    UWidgetAnimation* ResultVictoryScale;

    UPROPERTY(BlueprintReadWrite, Transient, meta = (BindWidgetAnim))
    UWidgetAnimation* ResultFailScale;

private:
    // 현재 배속 단계 (0: 1배, 1: 1.5배, 2: 2배)
    int32 CurrentSpeedIndex = 0;

    // 배속 값 배열
    const TArray<float> SpeedValues = { 1.0f, 1.5f, 2.0f };

    // 플레이타임
    float ElapsedPlayTime = 0.0f;

    // 자동 모드시 유닛 스폰 타이머 핸들
    FTimerHandle AutoSpawnTimerHandle;

    // 순차 유닛 스폰 인덱스
    int32 CurrentAutoSpawnIndex = 0;

    // 카메라 제어 함수
    bool bIsControlCamera = false;
    FVector2D DragStartPos;
};
