// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "AccessoryDataAsset.generated.h"
class UGameplayEffect;
/**
 * 
 */
UCLASS()
class UNREAL_PROJECTG_API UAccessoryDataAsset : public UDataAsset
{
	GENERATED_BODY()
public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GAS|Effects")
    TSubclassOf<UGameplayEffect> inviolabilityEffects;
};
