// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/PGGameplayAbility.h"
#include "PGUnitGameplayAbility.generated.h"

class AUnitCharacter;
class UUnitCombatComponent;
/**
 * 
 */
UCLASS()
class UNREAL_PROJECTG_API UPGUnitGameplayAbility : public UPGGameplayAbility
{
	GENERATED_BODY()
	
public:
    UFUNCTION(BlueprintPure, Category = "PG|Ability")
    AUnitCharacter* GetUnitCharacterFromActorInfo();

private:
    TWeakObjectPtr<AUnitCharacter> CachedUnitCharacter;
};
