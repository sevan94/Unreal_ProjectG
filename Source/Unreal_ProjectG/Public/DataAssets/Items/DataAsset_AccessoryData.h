// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "DataAsset_AccessoryData.generated.h"

class UPGGameplayAbility;
class UDataAsset_SkillData;
/**
 * 
 */
UCLASS()
class UNREAL_PROJECTG_API UDataAsset_AccessoryData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
    //FORCEINLINE const TSubclassOf<UPGGameplayAbility>& GetGrantedAbility() const { return AbilityToGrant; }

public:
    // 세트 구분을 위한 태그
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (Categories = "Set"))
    FGameplayTag SetTag;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
    TSoftObjectPtr<UDataAsset_SkillData> AccessoryAbilityData;
};
