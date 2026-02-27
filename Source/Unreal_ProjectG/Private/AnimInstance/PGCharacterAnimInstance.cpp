// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstance/PGCharacterAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Character/PGCharacterBase.h"

void UPGCharacterAnimInstance::NativeInitializeAnimation()
{
    // TODO : 캐릭터 타입을 우리의 캐릭터 타입으로 변경 변경
    OwningCharacter = Cast<APGCharacterBase>(TryGetPawnOwner());

    if (OwningCharacter.IsValid())
    {
        OwningCharacterMovement = OwningCharacter->GetCharacterMovement();
    }
}

void UPGCharacterAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
    if (!OwningCharacter.IsValid() || !OwningCharacterMovement)
    {
        return;
    }

    GroundSpeed = OwningCharacter->GetVelocity().Size2D();
}
