// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/DataTable.h"
#include "SkillUIDataTable.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FSkillUIDataTable : public FTableRowBase
{
	GENERATED_BODY()
	
public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    UTexture2D* SkillIcon = nullptr;
};
