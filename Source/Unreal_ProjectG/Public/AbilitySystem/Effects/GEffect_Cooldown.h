// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "GEffect_Cooldown.generated.h"

/**
 *  어빌리티의 쿨타임을 담당하는 GameplayEffect입니다.
 */
UCLASS()
class UNREAL_PROJECTG_API UGEffect_Cooldown : public UGameplayEffect
{
	GENERATED_BODY()
	
public:
    UGEffect_Cooldown();
};
