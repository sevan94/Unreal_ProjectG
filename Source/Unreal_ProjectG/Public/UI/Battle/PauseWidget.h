// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PauseWidget.generated.h"

class USlider;
class UButton;
class UPGGameInstance;

/**
 * 
 */
UCLASS()
class UNREAL_PROJECTG_API UPauseWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
    virtual void NativeConstruct() override;

    // 실제 게임 인스턴스에서 볼륨을 변경할 함수
    void UpdateVolumes();

    // 슬라이더 값이 변경될 때 호출될 함수들
    UFUNCTION()
    void OnMasterChanged(float Value);

    UFUNCTION()
    void OnBGMChanged(float Value);

    UFUNCTION()
    void OnSFXChanged(float Value);

    UFUNCTION()
    void OnVoiceChanged(float Value);

    // 버튼 이벤트
    UFUNCTION()
    void OnLobbyButtonClicked();

    UFUNCTION()
    void OnContinueButtonClicked();

protected:
    // 슬라이더 바인딩 (이미지 계층 구조 참조)
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<USlider> MasterVolume;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<USlider> BGMVolume;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<USlider> SFXVolume;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<USlider> VoiceVolume;

    // 버튼 바인딩
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> LobbyButton;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> ContinueButton;

    UPROPERTY(BlueprintReadOnly)
    TObjectPtr<UPGGameInstance> GI;
};
