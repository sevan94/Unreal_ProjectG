// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/DataTable.h"
#include "DataAssets/UI/EnemyUIDataAsset.h"
#include "DataAssets/Spawner/DA_StageUnitListDataAsset.h"
#include "StageDataTable.generated.h"

 /**
  *
  */
USTRUCT(BlueprintType)
struct FStageDataTable : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 StageCode;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText StageNumber;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 StarCount;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector2D MapPosition;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TObjectPtr<UDA_StageUnitListDataAsset> EnemyList;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSoftObjectPtr<UWorld> StageLevel;
};