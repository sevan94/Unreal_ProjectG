// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "GameplayTagContainer.h"
#include "PGBaseAnimInstance.generated.h"

/**
 *   애니메이션 인스턴스에서 공통으로 사용할 헬퍼 함수를 여기서 정의
 */
UCLASS()
class UNREAL_PROJECTG_API UPGBaseAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
protected:
    // 오너 액터가 특정 태그를 가지고 있는지 확인하는 헬퍼 함수
    UFUNCTION(BlueprintPure, meta = (BlueprintThreadSafe))
    bool DoesOwnerHaveTag(FGameplayTag TagToCheck) const;
};
