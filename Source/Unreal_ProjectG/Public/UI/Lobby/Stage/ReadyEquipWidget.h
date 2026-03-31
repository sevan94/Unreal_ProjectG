// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Types/PGEnumTypes.h"
#include "DataAssets/UI/EquipUIDataAsset.h"
#include "ReadyEquipWidget.generated.h"

class UImage;

/**
 * 
 */
UCLASS()
class UNREAL_PROJECTG_API UReadyEquipWidget : public UUserWidget
{
	GENERATED_BODY()

public:
    void UpdateEquipWidget(UEquipUIDataAsset* InData);

protected:
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UImage> EquipImage;
};
