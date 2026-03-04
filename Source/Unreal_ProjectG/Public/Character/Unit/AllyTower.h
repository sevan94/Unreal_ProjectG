// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Unit/UnitCharacter.h"
#include "AbilitySystemInterface.h" 
#include "Components/SphereComponent.h"
#include "GameplayTagContainer.h"
#include "Types/PGEnumTypes.h"
#include "GameplayEffectTypes.h"
#include "AllyTower.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAllyBaseDestroyed, ETeamType, DestroyedTeam);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAllyBaseHpChanged, FGameplayTag, TeamTag, float, CurrentHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAllyBaseMaxHpChanged, FGameplayTag, TeamTag, float, MaxHealth);
/**
 * 
 */
UCLASS()
class UNREAL_PROJECTG_API AAllyTower : public AUnitCharacter
{
	GENERATED_BODY()
public:
    AAllyTower();

protected:
    virtual void BeginPlay() override;
    virtual void OnDie() override;

public:
    UPROPERTY(BlueprintAssignable)
    FOnAllyBaseDestroyed OnBaseDestroyed;

    FOnAllyBaseHpChanged OnBaseHpChanged;
    FOnAllyBaseMaxHpChanged OnBaseMaxHpChanged;

    UPROPERTY(EditDefaultsOnly, Category = "GAS")
    TSubclassOf<class UGameplayEffect> InitStatEffect;
};
