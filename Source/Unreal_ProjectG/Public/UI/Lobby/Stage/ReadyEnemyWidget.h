// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ReadyEnemyWidget.generated.h"

class UImage;
class UEnemyUIDataAsset;

/**
 * 
 */
UCLASS()
class UNREAL_PROJECTG_API UReadyEnemyWidget : public UUserWidget
{
	GENERATED_BODY()

public:
    void UpdateEnemyWidget(UEnemyUIDataAsset* InData);
	
protected:
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UImage> EnemyImage;
};
