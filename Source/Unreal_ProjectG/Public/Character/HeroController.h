// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Interfaces/JoysticInput.h"
#include "HeroController.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_PROJECTG_API AHeroController : public APlayerController, public IJoysticInput
{
	GENERATED_BODY()

public:
    void MoveCamera(float DeltaY);
    void SaveCameraPosition();
    void ResetCameraPosition();
    void SetCameraClamp(float InMin, float InMax);

protected:
    virtual void OnPossess(APawn* InPawn) override;
    virtual void BeginPlay() override;
    virtual void PlayerTick(float DeltaTime) override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
    virtual void SetupInputComponent() override;
    virtual void MoveStart_Implementation(FVector2D JoyInput) override;
    virtual void ChangeDirection_Implementation(FVector2D JoyInput) override;
    virtual void EndMovement_Implementation() override;

private:
    void MoveCharacter();

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InputAction")
    TObjectPtr<class UInputMappingContext> InputMappingContext = nullptr;

private:
    //움직임 판정
    bool bIsMoving = false;

    //움직일 방향
    FVector MoveDirection = FVector::ZeroVector;

    // 카메라 컨트롤 변수
    bool bIsCameraManual = false;
    float CameraTargetY = 0.0f;
    float CameraBaseY = 0.0f;
    UPROPERTY(EditAnywhere, Category = "Camera")
    float MaxCameraOffset = 800.0f; // 좌우 최대 이동 범위
    UPROPERTY(EditAnywhere, Category = "Camera")
    float CameraReturnSpeed = 5.0f; // 복구 시 보간 속도
    float MapMin = 0.0f;
    float MapMax = 0.0f;

    UPROPERTY()
    TObjectPtr<class AHeroCharacter> Hero = nullptr;
};
