// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UnitDescriptionWidget.generated.h"

class UImage;
class UTextBlock;
class UUnitUIDataAsset;

/**
 * 
 */
UCLASS()
class UNREAL_PROJECTG_API UUnitDescriptionWidget : public UUserWidget
{
	GENERATED_BODY()

public:
    // 유닛 데이터가 들어오면 UI를 갱신합니다.
    UFUNCTION(BlueprintCallable)
    void UpdateDescription(UUnitUIDataAsset* InData);

protected:
    // 유닛 이미지
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UImage> UnitImage;

    // 유닛 이름
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> UnitName;

    // 유닛 레벨
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> UnitLevel;

    // 유닛 체력
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> UnitHealth;

    // 유닛 공격력
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> UnitAttackDamage;

    // 유닛 공격 속도
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> UnitAttackSpeed;

    // 유닛 이동속도
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> UnitMoveSpeed;

    // 유닛 코스트
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> UnitCost;

    // GE 수치를 읽기 위한 커브 테이블 (에디터에서 할당)
    UPROPERTY(EditDefaultsOnly, Category = "Data")
    TObjectPtr<UCurveTable> StatusCurveTable;
};
