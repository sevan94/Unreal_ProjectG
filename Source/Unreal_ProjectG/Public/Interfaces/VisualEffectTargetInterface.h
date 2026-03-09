// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "VisualEffectTargetInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UVisualEffectTargetInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class UNREAL_PROJECTG_API IVisualEffectTargetInterface
{
	GENERATED_BODY()

public:

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "IVisualEffectTargetInterface")
    void SetAOEHighlightEnabled(bool bEnabled);
    
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "IVisualEffectTargetInterface")
    void SetHitReactEnabled(bool bEnabled);

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "IVisualEffectTargetInterface")
    void ResetVisualEffectState();
};
