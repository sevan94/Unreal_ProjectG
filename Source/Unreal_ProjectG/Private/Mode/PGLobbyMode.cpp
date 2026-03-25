// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/PGLobbyMode.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "Runtime/CinematicCamera/Public/CineCameraActor.h"
#include "UI/Lobby/Gacha/GachaActor.h"
#include "UI/Lobby/Gacha/EquipGachaActor.h"
#include "UI/Lobby/Main/LobbyHUD.h"

void APGLobbyMode::PlayUnitGacha(UUnitUIDataAsset* InData)
{
    if (UnitGachaActor)
    {
        UnitGachaActor->GachaMove(InData);
    }
}

void APGLobbyMode::PlayEquipGacha()
{
    if (EquipGachaActor)
    {
        EquipGachaActor->ChestOpen();
    }
}

void APGLobbyMode::BeginPlay()
{
    Super::BeginPlay();

    // 레벨에서 장비 가챠 액터를 찾아 저장
    EquipGachaActor = Cast<AEquipGachaActor>(UGameplayStatics::GetActorOfClass(GetWorld(), AEquipGachaActor::StaticClass()));
    if (EquipGachaActor)
    {
        // 상자 애니메이션이 끝났을 때의 델리게이트 바인딩
        EquipGachaActor->OnChestOpenFinished.AddDynamic(this, &APGLobbyMode::OnChestAnimationFinished);
    }

    // 가챠 액터 스폰
    if (GachaActorClass)
    {
        FActorSpawnParameters SpawnParams;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

        UnitGachaActor = GetWorld()->SpawnActor<AGachaActor>(GachaActorClass, GachaStartTransform, SpawnParams);
        UnitGachaActor->OnGachaOpenFinished.AddDynamic(this, &APGLobbyMode::OnGachaAnimationFinished);
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
    if (CineCam && UnitGachaActor)
    {
        FCameraLookatTrackingSettings TrackingSettings;
        TrackingSettings.bEnableLookAtTracking = true;
        TrackingSettings.LookAtTrackingInterpSpeed = 5.0f;
        TrackingSettings.ActorToTrack = UnitGachaActor;
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
            LobbyHUD->ShowUnitGachaResultUI();
        }
    }
}

void APGLobbyMode::OnChestAnimationFinished()
{
    // HUD를 찾아 뽑기 결과창을 띄움
    APlayerController* PC = GetWorld()->GetFirstPlayerController();
    if (PC && PC->GetHUD())
    {
        ALobbyHUD* LobbyHUD = Cast<ALobbyHUD>(PC->GetHUD());
        if (LobbyHUD)
        {
            LobbyHUD->ShowEquipGachaResultUI();
        }
    }
}
