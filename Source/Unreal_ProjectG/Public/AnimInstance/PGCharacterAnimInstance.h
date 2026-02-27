// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimInstance/PGBaseAnimInstance.h"
#include "PGCharacterAnimInstance.generated.h"

class UCharacterMovementComponent;
class APGCharacterBase;
/**
 *   유닛과 캐릭터가 공통으로 사용할 변수를 여기서 정의
 */
UCLASS()
class UNREAL_PROJECTG_API UPGCharacterAnimInstance : public UPGBaseAnimInstance
{
	GENERATED_BODY()
	
public:
    virtual void NativeInitializeAnimation() override;

    virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;

protected:
    // 오너 캐릭터 캐싱
    UPROPERTY()
    TWeakObjectPtr<APGCharacterBase> OwningCharacter;

    // 오너 캐릭터의 무브먼트 컴포넌트 캐싱
    UPROPERTY()
    TObjectPtr<UCharacterMovementComponent> OwningCharacterMovement;

    // 캐릭터의 지상 이동 속도
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimData|LocomotionData")
    float GroundSpeed;
};
