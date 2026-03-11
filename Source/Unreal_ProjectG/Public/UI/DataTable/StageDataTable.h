// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/DataTable.h"
#include "StageDataTable.generated.h"

 /**
  *
  */
USTRUCT(BlueprintType)
struct FStageDataTable : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText StageNumber;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 StarCount;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector2D MapPosition;
};