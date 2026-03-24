// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "PGDataTableStruct.generated.h"

/**
 * 데이터 테이블 구조체 모음
 */

USTRUCT(BlueprintType)
struct FSpawnOffsetRow : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "소환 오프셋"))
    FVector SpawnOffset = FVector::ZeroVector;
};