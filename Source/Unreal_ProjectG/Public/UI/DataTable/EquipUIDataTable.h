// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "Engine/DataTable.h"
#include "DataAssets/UI/EquipUIDataAsset.h"
#include "EquipUIDataTable.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FEquipUIDataTable : public FTableRowBase
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 EquipID;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TObjectPtr<UEquipUIDataAsset> EquipData;
};