// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "HeroController.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_PROJECTG_API AHeroController : public APlayerController
{
	GENERATED_BODY()

public:
    // 조이스틱 할당
    void SetJoystickWidget(class UControlPanelWidget* InWidget) { JoystickWidget = InWidget; }
	
protected:
    virtual void OnPossess(APawn* InPawn) override;
    virtual void BeginPlay() override;
    virtual void PlayerTick(float DeltaTime) override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
    virtual void SetupInputComponent() override;

protected:
    // 조이스틱
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
    TObjectPtr<class UControlPanelWidget> JoystickWidget = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InputAction")
    TObjectPtr<class UInputMappingContext> InputMappingContext = nullptr;
};
