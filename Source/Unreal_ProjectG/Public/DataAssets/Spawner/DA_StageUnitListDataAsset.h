// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DA_StageUnitListDataAsset.generated.h"
class AUnitCharacter;
class UUnitUIDataAsset;

USTRUCT(BlueprintType)
struct FUnitSpawnDataInfo
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
    TObjectPtr<UUnitUIDataAsset> UnitData = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
    int32 Spawnweight = 1;
};

USTRUCT(BlueprintType)
struct FStageInfo
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
    int32 StageIndex = 1;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
    TArray<FUnitSpawnDataInfo> UnitSpawnList;
};
/**
 * 
 */

UCLASS()
class UNREAL_PROJECTG_API UDA_StageUnitListDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
    UPROPERTY(EditAnywhere, Category = "Stage")
    TArray<FStageInfo> Stage;

};
