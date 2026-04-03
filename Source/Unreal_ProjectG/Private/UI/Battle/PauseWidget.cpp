// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Battle/PauseWidget.h"
#include "Components/Slider.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Mode/Save/PGGameInstance.h"

void UPauseWidget::NativeConstruct()
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
        if (VoiceVolume) VoiceVolume->SetValue(GI->CurrentVoiceVolume);
    }

    // 볼륨 슬라이더 이벤트 연결
    if (MasterVolume) MasterVolume->OnValueChanged.AddDynamic(this, &UPauseWidget::OnMasterChanged);
    if (BGMVolume) BGMVolume->OnValueChanged.AddDynamic(this, &UPauseWidget::OnBGMChanged);
    if (SFXVolume) SFXVolume->OnValueChanged.AddDynamic(this, &UPauseWidget::OnSFXChanged);
    if (VoiceVolume) VoiceVolume->OnValueChanged.AddDynamic(this, &UPauseWidget::OnVoiceChanged);

    // 버튼 이벤트 연결
    if (LobbyButton) LobbyButton->OnClicked.AddDynamic(this, &UPauseWidget::OnLobbyButtonClicked);
    if (ContinueButton) ContinueButton->OnClicked.AddDynamic(this, &UPauseWidget::OnContinueButtonClicked);
}

void UPauseWidget::UpdateVolumes()
{
    if (GI)
    {
        GI->SetSoundVolumes(GI->CurrentMasterVolume, GI->CurrentBGMVolume, GI->CurrentSFXVolume, GI->CurrentVoiceVolume);
    }
}

void UPauseWidget::OnMasterChanged(float Value)
{
    if (GI)
    {
        GI->CurrentMasterVolume = Value;
        UpdateVolumes();
    }
}

void UPauseWidget::OnBGMChanged(float Value)
{
    if (GI)
    {
        GI->CurrentBGMVolume = Value;
        UpdateVolumes();
    }
}

void UPauseWidget::OnSFXChanged(float Value)
{
    if (GI)
    {
        GI->CurrentSFXVolume = Value;
        UpdateVolumes();
    }
}

void UPauseWidget::OnVoiceChanged(float Value)
{
    if (GI)
    {
        GI->CurrentVoiceVolume = Value;
        UpdateVolumes();
    }
}

void UPauseWidget::OnLobbyButtonClicked()
{
    // 일시정지 해제 후 로비 맵으로 이동
    UGameplayStatics::SetGamePaused(GetWorld(), false);
    UGameplayStatics::OpenLevel(GetWorld(), TEXT("Lobby"));
}

void UPauseWidget::OnContinueButtonClicked()
{
    // 일시정지 해제 및 위젯 숨기기
    UGameplayStatics::SetGamePaused(GetWorld(), false);
    SetVisibility(ESlateVisibility::Collapsed);
}
