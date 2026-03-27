// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstance/PGCharacterAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Character/PGCharacterBase.h"
#include "Math/RotationMatrix.h"

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

    const FVector Velocity = OwningCharacterMovement->Velocity;
    GroundSpeed = Velocity.Size2D();


    //Direction = UKismetAnimationLibrary::CalculateDirection(Velocity, OwningCharacter->GetActorRotation()); // <= KismetAnimationLibrary를 포함 시켜야함 
    const FRotator ActorRotation = OwningCharacter->GetActorRotation();
    const FVector Forward = FRotationMatrix(ActorRotation).GetScaledAxis(EAxis::X);
    const FVector Right = FRotationMatrix(ActorRotation).GetScaledAxis(EAxis::Y);
    const FVector MoveDir = Velocity.GetSafeNormal2D();

    const float ForwardCos = FVector::DotProduct(Forward, MoveDir);
    const float RightCos = FVector::DotProduct(Right, MoveDir);
    Direction = FMath::RadiansToDegrees(FMath::Atan2(RightCos, ForwardCos));
}
