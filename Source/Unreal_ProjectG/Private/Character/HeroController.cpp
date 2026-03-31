// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/HeroController.h"
#include "UI/Battle/ControlPanelWidget.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubSystems.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraActor.h"
#include "Character/Hero/HeroCharacter.h"
#include "AbilitySystemComponent.h"
#include "PGGameplayTags.h"

// 내부 헬퍼 함수
namespace HeroController
{
    // ASC가 State_InputBlock_Move 태그를 가지고 있는지 확인하는 함수
    bool IsMoveBlockedByGameplayTag(const AHeroCharacter* Hero)
    {
        if (!Hero)
        {
            return false;
        }

        const UAbilitySystemComponent* ASC = Hero->GetAbilitySystemComponent();
        return ASC && ASC->HasMatchingGameplayTag(PGGameplayTags::State_InputBlock_Move);
    }
}

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

    SetShowMouseCursor(true);
    Hero = Cast<AHeroCharacter>(GetPawn());
}

void AHeroController::PlayerTick(float DeltaTime)
{
    Super::PlayerTick(DeltaTime);

    // 조이스틱 위젯이 있고, 입력값이 있다면 이동 처리
    if (Hero && bIsMoving)
    {
        MoveCharacter();
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

void AHeroController::MoveStart_Implementation(FVector2D JoyInput)
{
    if (HeroController::IsMoveBlockedByGameplayTag(Hero))
    {
        bIsMoving = false;
        return;
    }

    bIsMoving = true;

    MoveDirection = FVector(JoyInput.X, JoyInput.Y, 0);
}

void AHeroController::ChangeDirection_Implementation(FVector2D JoyInput)
{
    if (HeroController::IsMoveBlockedByGameplayTag(Hero))
    {
        bIsMoving = false;
        return;
    }

    MoveDirection = FVector(JoyInput.X, JoyInput.Y, 0);
}

void AHeroController::EndMovement_Implementation()
{
    bIsMoving = false;
}

void AHeroController::MoveCharacter()
{
    if (HeroController::IsMoveBlockedByGameplayTag(Hero))
    {
        bIsMoving = false;
        return;
    }

    Hero->AddMovementInput(MoveDirection);
}
