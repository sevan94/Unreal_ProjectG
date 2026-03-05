// Fill out your copyright notice in the Description page of Project Settings.
#include "AnimInstance/PGPawnAnimInstance.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Pawn/PGPetBase.h"

void UPGPawnAnimInstance::NativeInitializeAnimation()
{
    OwningPet = Cast<APGPetBase>(TryGetPawnOwner());

    if (OwningPet.IsValid())
    {
        OwningPetMovement = OwningPet->GetMovementComponent();
    }
}

void UPGPawnAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
    if(!OwningPet.IsValid() || !OwningPetMovement)
    {
        return;
    }

    Speed = OwningPet->GetVelocity().Size();
}
