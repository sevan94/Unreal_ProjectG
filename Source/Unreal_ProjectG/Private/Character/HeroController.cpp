// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/HeroController.h"
#include "UI/Battle/ControlPanelWidget.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubSystems.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraActor.h"

void AHeroController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);
}

void AHeroController::BeginPlay()
{
    Super::BeginPlay();

    AActor* Camera = UGameplayStatics::GetActorOfClass(GetWorld(), ACameraActor::StaticClass());

    if (Camera)
    {
        this->SetViewTargetWithBlend(Camera, 0.0f);
    }
}

void AHeroController::PlayerTick(float DeltaTime)
{
    Super::PlayerTick(DeltaTime);

    // 조이스틱 위젯이 있고, 입력값이 있다면 이동 처리
    if (JoystickWidget && GetPawn())
    {
        FVector2D JoyInput = JoystickWidget->GetJoystickVector();
        //UE_LOG(LogTemp, Log, TEXT("조이스틱 위젯 확인"));

        if (!JoyInput.IsNearlyZero())
        {
            const FVector ForwardDirection = FVector::ForwardVector;
            const FVector RightDirection = FVector::RightVector;

            // 위젯 좌표계와 월드 좌표계 매칭 (상황에 따라 Y축 반전 필요할 수 있음)
            GetPawn()->AddMovementInput(ForwardDirection, JoyInput.X);
            GetPawn()->AddMovementInput(RightDirection, JoyInput.Y);

            //UE_LOG(LogTemp, Log, TEXT("JoyInput: X=%.2f, Y=%.2f"), JoyInput.X, JoyInput.Y);
        }
    }
}

void AHeroController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);
}

void AHeroController::SetupInputComponent()
{
    Super::SetupInputComponent();

    UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());

    if (Subsystem)
    {
        UE_LOG(LogTemp, Log, TEXT("Player Controller Subsystem Available"));
        Subsystem->AddMappingContext(InputMappingContext, 1);
    }
}
