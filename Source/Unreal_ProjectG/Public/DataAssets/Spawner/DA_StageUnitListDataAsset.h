// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DA_StageUnitListDataAsset.generated.h"

class AUnitCharacter;
class UEnemyUIDataAsset;

USTRUCT(BlueprintType)
struct FUnitSpawnDataInfo
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
    TObjectPtr<UEnemyUIDataAsset> UnitData;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
    int32 Spawnweight = 1;
};

UCLASS()
class UNREAL_PROJECTG_API UDA_StageUnitListDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
    UPROPERTY(EditAnywhere, Category = "Stage")
    TArray<FUnitSpawnDataInfo> UnitSpawnList;

};
