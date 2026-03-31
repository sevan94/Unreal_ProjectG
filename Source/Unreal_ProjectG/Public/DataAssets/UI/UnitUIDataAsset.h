// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Character/Unit/UnitCharacter.h"
#include "Types/PGEnumTypes.h"
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
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Unit|Data")
    int32 UnitID;

    // 유닛 등급
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Unit|Data")
    EUnitRank UnitRank;

    // 유닛 이름
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Unit|Data")
    FName UnitName;

    // 유닛 캐릭터 블루프린트
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Unit|Data")
    TSubclassOf<AUnitCharacter> UnitClass;

    // 유닛 스테이터스 커브테이블
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Unit|Data")
    TObjectPtr<UCurveTable> UnitStatus;

    // 유닛 타입 이미지
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Unit|Detail")
    TObjectPtr<UTexture2D> UnitTypeImage;

    // 유닛 등급 이미지
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Unit|Detail")
    TObjectPtr<UTexture2D> UnitRankImage;

    // 유닛 이미지
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Unit|Detail")
    TObjectPtr<UTexture2D> UnitImage;

    // 획득 사운드
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Unit|Detail")
    TObjectPtr<USoundBase> GachaSound;

    // 편성 사운드
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Unit|Detail")
    TObjectPtr<USoundBase> PartySound;

    // 유닛 설명
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Unit|Detail")
    FString UnitDetail;

    // 유닛 코스트
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Unit|Cost")
    int32 UnitCost;

    // 유닛 해금 비용
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Unit|Cost")
    int32 UnitUnlock;

    // 중복 획득 시 획득 재화
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Unit|Cost")
    int32 DuplicateUnitReward;
};
