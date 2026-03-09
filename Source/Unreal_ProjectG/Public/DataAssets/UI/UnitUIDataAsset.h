// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Character/Unit/UnitCharacter.h"
#include "UnitUIDataAsset.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class UNREAL_PROJECTG_API UUnitUIDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
    // 유닛 구분용 ID
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Unit")
    int32 UnitID;

    // 유닛 캐릭터 블루프린트
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Unit")
    TSubclassOf<AUnitCharacter> UnitClass;

    // 유닛 버튼 이미지
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Unit")
    TObjectPtr<UTexture2D> UnitImage;

    // 유닛 이름
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Unit")
    FName UnitName;

    // 유닛 스테이터스 식별용 이름
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Unit")
    FString UnitStatus;

    // 유닛 코스트
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Unit")
    int32 UnitCost;
};
