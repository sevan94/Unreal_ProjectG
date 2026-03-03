// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Combat/PawnCombatComponent.h"
#include "UnitCombatComponent.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_PROJECTG_API UUnitCombatComponent : public UPawnCombatComponent
{
	GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TArray<UAnimMontage*> BasicAttackMontages;
};
