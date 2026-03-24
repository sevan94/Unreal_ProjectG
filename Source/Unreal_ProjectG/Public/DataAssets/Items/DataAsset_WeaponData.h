// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Types/PGStructTypes.h"
#include "DataAsset_WeaponData.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_PROJECTG_API UDataAsset_WeaponData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
    FORCEINLINE const FPGHeroWeaponData& GetHeroWeaponData() const { return HeroWeaponData; }

public:
    // 세트 구분을 위한 태그
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (Categories = "Set"))
    FGameplayTag SetTag;

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FPGHeroWeaponData HeroWeaponData;
};
