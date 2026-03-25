// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "LobbyHUD.generated.h"

class UUISwitcherWidget;

/**
 * 
 */
UCLASS()
class UNREAL_PROJECTG_API ALobbyHUD : public AHUD
{
	GENERATED_BODY()

public:
    void ShowUnitGachaResultUI();
    void ShowEquipGachaResultUI();

protected:
    virtual void BeginPlay() override;

protected:
    UPROPERTY(EditAnywhere, Category = "UI")
    TSubclassOf<UUserWidget> UISwitcher;

    UPROPERTY()
    TObjectPtr<UUISwitcherWidget> UISwitcherInstance;
};
