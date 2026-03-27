// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Battle/PauseWidget.h"
#include "Components/Slider.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

void UPauseWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // 볼륨 슬라이더 이벤트 연결
    if (MasterVolume) MasterVolume->OnValueChanged.AddDynamic(this, &UPauseWidget::OnMasterVolumeChanged);
    if (BGMVolume) BGMVolume->OnValueChanged.AddDynamic(this, &UPauseWidget::OnBGMVolumeChanged);
    if (SFXVolume) SFXVolume->OnValueChanged.AddDynamic(this, &UPauseWidget::OnSFXVolumeChanged);

    // 버튼 이벤트 연결
    if (LobbyButton) LobbyButton->OnClicked.AddDynamic(this, &UPauseWidget::OnLobbyButtonClicked);
    if (ContinueButton) ContinueButton->OnClicked.AddDynamic(this, &UPauseWidget::OnContinueButtonClicked);
}

void UPauseWidget::OnMasterVolumeChanged(float Value)
{
    // 볼륨 조절
}

void UPauseWidget::OnBGMVolumeChanged(float Value)
{
}

void UPauseWidget::OnSFXVolumeChanged(float Value)
{
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
