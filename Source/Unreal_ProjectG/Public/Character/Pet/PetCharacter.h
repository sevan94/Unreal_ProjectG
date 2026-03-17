// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/PGCharacterBase.h"
#include "PetCharacter.generated.h"

class UDataAsset_PetStartUpData;
/**
 * 
 */
UCLASS()
class UNREAL_PROJECTG_API APetCharacter : public APGCharacterBase
{
	GENERATED_BODY()
	
public:
    APetCharacter();

protected:
    virtual void PossessedBy(AController* NewController) override;

private:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    float AttackPowerMultiplier = 0.75f; // 공격력 배율
};
