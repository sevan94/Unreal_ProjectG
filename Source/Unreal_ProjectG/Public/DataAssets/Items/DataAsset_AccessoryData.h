// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DataAsset_AccessoryData.generated.h"

class UPGGameplayAbility;
/**
 * 
 */
UCLASS()
class UNREAL_PROJECTG_API UDataAsset_AccessoryData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
    FORCEINLINE const TSubclassOf<UPGGameplayAbility>& GetGrantedAbility() const { return AbilityToGrant; }
protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
    TSubclassOf<UPGGameplayAbility> AbilityToGrant;
};
