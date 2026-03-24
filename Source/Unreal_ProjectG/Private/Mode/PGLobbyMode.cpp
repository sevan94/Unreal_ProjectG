// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/PGLobbyMode.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "Runtime/CinematicCamera/Public/CineCameraActor.h"
#include "UI/Lobby/Gacha/GachaActor.h"
#include "UI/Lobby/Main/LobbyHUD.h"

void APGLobbyMode::PlayGacha(UUnitUIDataAsset* InData)
{
    if (GachaActor)
    {
        GachaActor->GachaMove(InData);
    }
}

void APGLobbyMode::BeginPlay()
{
    Super::BeginPlay();

    // 가챠 액터 스폰
    if (GachaActorClass)
    {
        FActorSpawnParameters SpawnParams;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

        GachaActor = GetWorld()->SpawnActor<AGachaActor>(GachaActorClass, GachaStartTransform, SpawnParams);
        GachaActor->OnGachaOpenFinished.AddDynamic(this, &APGLobbyMode::OnGachaAnimationFinished);
    }

    // 카메라 할당
    TargetCamera = UGameplayStatics::GetActorOfClass(GetWorld(), ACineCameraActor::StaticClass());

    // 카메라 시점으로 전환
    APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (PC && TargetCamera)
    {
        PC->SetViewTargetWithBlend(TargetCamera, 0.0f);
    }

    // 카메라 트래킹 설정
    ACineCameraActor* CineCam = Cast<ACineCameraActor>(TargetCamera);
    if (CineCam && GachaActor)
    {
        FCameraLookatTrackingSettings TrackingSettings;
        TrackingSettings.bEnableLookAtTracking = true;
        TrackingSettings.LookAtTrackingInterpSpeed = 5.0f;
        TrackingSettings.ActorToTrack = GachaActor;
        TrackingSettings.bAllowRoll = false;

        CineCam->LookatTrackingSettings = TrackingSettings;
    }

    PC->bShowMouseCursor = true;
    PC->bEnableClickEvents = true;
    PC->bEnableTouchEvents = true;
}

void APGLobbyMode::OnGachaAnimationFinished()
{
    // HUD를 찾아 뽑기 결과창을 띄움
    APlayerController* PC = GetWorld()->GetFirstPlayerController();
    if (PC && PC->GetHUD())
    {
        ALobbyHUD* LobbyHUD = Cast<ALobbyHUD>(PC->GetHUD());
        if (LobbyHUD)
        {
            LobbyHUD->ShowGachaResultUI();
        }
    }
}