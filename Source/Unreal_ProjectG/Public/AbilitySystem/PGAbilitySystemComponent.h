// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "PGAbilitySystemComponent.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_PROJECTG_API UPGAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
    // 태그로 어빌리티 활성화 시도
    UFUNCTION(BlueprintCallable, Category = "Abilities")
    bool TryActivateAbilityByTag(FGameplayTag AbilityTagToActivate);
};
