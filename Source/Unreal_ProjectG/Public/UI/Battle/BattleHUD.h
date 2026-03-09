// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "BattleHUD.generated.h"

class UBattleUIWidget;
struct FBattleResultData;

/**
 * 
 */
UCLASS()
class UNREAL_PROJECTG_API ABattleHUD : public AHUD
{
	GENERATED_BODY()

public:
    void OnGameOver(const FBattleResultData& ResultData);

    void SetAbilitySpec();

protected:
    virtual void BeginPlay() override;

protected:
    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "HUD")
    TSubclassOf<UUserWidget> HUDWidgetClass = nullptr;

    UPROPERTY(BlueprintReadOnly, Category = "HUD")    
    TObjectPtr<UBattleUIWidget> BattleUIWidget = nullptr;
};
