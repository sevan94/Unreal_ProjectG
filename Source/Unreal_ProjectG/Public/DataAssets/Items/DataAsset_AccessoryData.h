// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "DataAsset_AccessoryData.generated.h"

class UPGHeroSkillGameplayAbility;
class UDataAsset_HeroSkillData;
/**
 * 
 */
UCLASS()
class UNREAL_PROJECTG_API UDataAsset_AccessoryData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
    //FORCEINLINE const TSubclassOf<UPGGameplayAbility>& GetGrantedAbility() const { return AbilityToGrant; }
    bool IsValid() const;

public:
    // 세트 구분을 위한 태그
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (Categories = "Equipment.Set"))
    FGameplayTag SetTag;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
    TSubclassOf<UPGHeroSkillGameplayAbility> AccessoryAbilityClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
    TSoftObjectPtr<UDataAsset_HeroSkillData> AccessoryAbilityData;
};
