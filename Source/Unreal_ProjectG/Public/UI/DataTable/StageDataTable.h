// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/DataTable.h"
#include "DataAssets/UI/EnemyUIDataAsset.h"
#include "DataAssets/Spawner/DA_StageUnitListDataAsset.h"
#include "Types/PGEnumTypes.h"
#include "StageDataTable.generated.h"

 /**
  *
  */
USTRUCT(BlueprintType)
struct FStageDataTable : public FTableRowBase
{
    GENERATED_BODY()

    // 스테이지 코드
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StageInfo")
    int32 StageCode = 0;

    // 출력할 스테이지 단계
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StageInfo")
    FText StageNumber;

    // 지도에 배치될 좌표
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StageInfo")
    FVector2D MapPosition = FVector2D::ZeroVector;

    // 오픈할 레벨
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StageInfo")
    TSoftObjectPtr<UWorld> StageLevel;

    // 출현할 적 정보
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StageDetail")
    TObjectPtr<UDA_StageUnitListDataAsset> EnemyList;

    // 스테이지 달성도 분류
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StageDetail")
    ERewardCategory RewardType = ERewardCategory::Cost;

    // 2별 조건
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StageDetail")
    float Star2 = 0.0f;
    // 3별 조건
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StageDetail")
    float Star3 = 0.0f;

    // 보상 젬
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StageDetail")
    int32 RewardGem = 0;
    // 보상 골드
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StageDetail")
    int32 RewardGold = 0;

    // 테스트용 달성도 개수
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test")
    int32 StarCount = 0;
};