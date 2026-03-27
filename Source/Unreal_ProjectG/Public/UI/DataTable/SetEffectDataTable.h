// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GameplayTagContainer.h"
#include "SetEffectDataTable.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FSetEffectDataTable : public FTableRowBase
{
    GENERATED_BODY()

public:
    // 식별용 세트 태그 (EquipUIDataAsset의 SetTag와 비교군)
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FGameplayTag SetTag;

    // 활성화에 필요한 최소 개수
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 RequiredCount = 2;

    // UI 표시용 이름
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FText SetName;

    // UI 표시용 아이콘
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TObjectPtr<UTexture2D> SetIcon;

    // 효과 설명 텍스트
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FText SetDescription;
};
