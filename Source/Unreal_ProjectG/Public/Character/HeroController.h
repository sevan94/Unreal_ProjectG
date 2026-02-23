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

    UPROPERTY()
    TObjectPtr<class AHeroCharacter> Hero = nullptr;
};
