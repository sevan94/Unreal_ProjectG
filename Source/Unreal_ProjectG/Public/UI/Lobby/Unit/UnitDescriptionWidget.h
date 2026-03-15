// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Mode/Save/PGGameInstance.h"
#include "UnitDescriptionWidget.generated.h"

class UImage;
class UButton;
class UTextBlock;
class UUnitEntryObject;
class UUnitUIDataAsset;

/**
 * 
 */
UCLASS()
class UNREAL_PROJECTG_API UUnitDescriptionWidget : public UUserWidget
{
	GENERATED_BODY()

public:
    virtual void NativeConstruct() override;

    // 유닛 데이터가 들어오면 UI를 갱신합니다.
    UFUNCTION(BlueprintCallable)
    void UpdateDescription(UUnitEntryObject* InEntryObject);

protected:
    // 버튼 클릭 함수
    UFUNCTION()
    void OnUpgradeButtonClicked();

    void SetUnitStatus();

protected:
    // 위젯에 표시할 UI 데이터 캐싱
    UPROPERTY()
    TObjectPtr<UUnitUIDataAsset> CurrentUIData;

    // 현재 위젯에 표시 중인 유닛 데이터 오브젝트
    UPROPERTY()
    TObjectPtr<UUnitEntryObject> CurrentEntryObject;

    // 레벨을 가져오기 위한 세이브 데이터
    UPROPERTY()
    FUnitSaveData SaveData;

    // GE 수치를 읽기 위한 커브 테이블 (에디터에서 할당)
    UPROPERTY(EditDefaultsOnly, Category = "Data")
    TObjectPtr<UCurveTable> StatusCurveTable;

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

    // 유닛 설명
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> UnitDetail;

    // 강화 버튼
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> UpgradeButton;
};
