// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimInstance/PGBaseAnimInstance.h"
#include "PGPawnAnimInstance.generated.h"

class UPawnMovementComponent;
class APGPetBase;
/**
 * 
 */
UCLASS()
class UNREAL_PROJECTG_API UPGPawnAnimInstance : public UPGBaseAnimInstance
{
	GENERATED_BODY()
	
public:
    virtual void NativeInitializeAnimation() override;
    virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;

protected:
    UPROPERTY()
    TWeakObjectPtr<APGPetBase> OwningPet;

    UPROPERTY()
    TObjectPtr<UPawnMovementComponent> OwningPetMovement;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimData|LocomotionData")
    float Speed;
};
