// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OptionWidget.generated.h"

class USlider;
class UButton;
class UPGGameInstance;

/**
 * 
 */
UCLASS()
class UNREAL_PROJECTG_API UOptionWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
    virtual void NativeConstruct() override;

private:
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

    UFUNCTION()
    void OnExitClicked();

protected:
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<USlider> MasterVolume;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<USlider> BGMVolume;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<USlider> SFXVolume;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<USlider> VoiceVolume;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> ExitButton;

private:
    UPROPERTY()
    TObjectPtr<UPGGameInstance> GI;
};
