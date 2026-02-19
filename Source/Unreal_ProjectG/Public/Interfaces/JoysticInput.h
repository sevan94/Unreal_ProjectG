// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "JoysticInput.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UJoysticInput : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class UNREAL_PROJECTG_API IJoysticInput
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
    //영웅 캐릭터 이동
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Movement")
    void MoveStart(FVector2D JoyInput);

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Movement")
    void ChangeDirection(FVector2D JoyInput);

    //영웅 캐릭터 정지
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Movement")
    void EndMovement();
};
