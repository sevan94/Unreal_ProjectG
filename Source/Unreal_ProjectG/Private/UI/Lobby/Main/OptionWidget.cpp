// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Lobby/Main/OptionWidget.h"
#include "Components/Slider.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Mode/Save/PGGameInstance.h"

void UOptionWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // GameInstance 가져오기
    GI = Cast<UPGGameInstance>(GetGameInstance());

    if (GI)
    {
        // 현재 GameInstance에 저장된 값을 슬라이더에 반영
        if (MasterVolume) MasterVolume->SetValue(GI->CurrentMasterVolume);
        if (BGMVolume) BGMVolume->SetValue(GI->CurrentBGMVolume);
        if (SFXVolume) SFXVolume->SetValue(GI->CurrentSFXVolume);
        if (VoiceVolume) SFXVolume->SetValue(GI->CurrentSFXVolume);
    }

    // 슬라이더 이벤트 연결
    if (MasterVolume) MasterVolume->OnValueChanged.AddDynamic(this, &UOptionWidget::OnMasterChanged);
    if (BGMVolume) BGMVolume->OnValueChanged.AddDynamic(this, &UOptionWidget::OnBGMChanged);
    if (SFXVolume) SFXVolume->OnValueChanged.AddDynamic(this, &UOptionWidget::OnSFXChanged);
    if (VoiceVolume) VoiceVolume->OnValueChanged.AddDynamic(this, &UOptionWidget::OnVoiceChanged);

    if (ExitButton) ExitButton->OnClicked.AddDynamic(this, &UOptionWidget::OnExitClicked);

}

void UOptionWidget::UpdateVolumes()
{
    if (GI)
    {
        GI->SetSoundVolumes(GI->CurrentMasterVolume, GI->CurrentBGMVolume, GI->CurrentSFXVolume, GI->CurrentVoiceVolume);
    }
}

void UOptionWidget::OnMasterChanged(float Value)
{
    if (GI)
    {
        GI->CurrentMasterVolume = Value;
        UpdateVolumes();
    }
}

void UOptionWidget::OnBGMChanged(float Value)
{
    if (GI)
    {
        GI->CurrentBGMVolume = Value;
        UpdateVolumes();
    }
}

void UOptionWidget::OnSFXChanged(float Value)
{
    if (GI)
    {
        GI->CurrentSFXVolume = Value;
        UpdateVolumes();
    }
}

void UOptionWidget::OnVoiceChanged(float Value)
{
    if (GI)
    {
        GI->CurrentVoiceVolume = Value;
        UpdateVolumes();
    }
}


void UOptionWidget::OnExitClicked()
{
    // 설정을 나갈 때 데이터를 저장
    if (GI)
    {
        GI->SaveGameData();
    }
    this->SetVisibility(ESlateVisibility::Hidden);
}