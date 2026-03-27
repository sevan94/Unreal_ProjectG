// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PauseWidget.generated.h"

class USlider;
class UButton;

/**
 * 
 */
UCLASS()
class UNREAL_PROJECTG_API UPauseWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
    virtual void NativeConstruct() override;

    // 슬라이더 이벤트
    UFUNCTION()
    void OnMasterVolumeChanged(float Value);

    UFUNCTION()
    void OnBGMVolumeChanged(float Value);

    UFUNCTION()
    void OnSFXVolumeChanged(float Value);

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

    // 버튼 바인딩
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> LobbyButton;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> ContinueButton;
};
