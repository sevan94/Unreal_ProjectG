// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/DataTable.h"
#include "DataAssets/UI/UnitUIDataAsset.h"
#include "UnitUIDataTable.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FUnitUIDataTable : public FTableRowBase
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TObjectPtr<UUnitUIDataAsset> UnitData = nullptr;
};
