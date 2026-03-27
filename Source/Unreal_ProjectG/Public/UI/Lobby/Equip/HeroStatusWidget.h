// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HeroStatusWidget.generated.h"

class UTextBlock;
class UButton;
class UGoodsBarWidget;
class UPGGameInstance;

/**
 * 
 */
UCLASS()
class UNREAL_PROJECTG_API UHeroStatusWidget : public UUserWidget
{
	GENERATED_BODY()

public:
    void SetHeroStatus();
    
protected:
    virtual void NativeConstruct() override;

    // 버튼 클릭 함수
    UFUNCTION()
    void OnUpgradeButtonClicked();

protected:
    // GE 수치를 읽기 위한 커브 테이블 (에디터에서 할당)
    UPROPERTY(EditDefaultsOnly, Category = "Data")
    TObjectPtr<UCurveTable> StatusCurveTable;

    // 영웅 레벨
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> HeroLevel;

    // 영웅 체력
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> HeroHealth;

    // 영웅 공격력
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> HeroAttackPower;

    // 영웅 공격 속도
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> HeroAttackSpeed;

    // 영웅 최대 코스트
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> HeroMaxCost;

    // 영웅 코스트 재생량
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> HeroCostRegenAmount;

    // 영웅 코스트 재생 시간
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> HeroCostRegenInterval;

    // 소모 비용 위젯
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UGoodsBarWidget> HeroUpgradeCost;

    // 강화 버튼
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> UpgradeButton;

private:
    TObjectPtr<UPGGameInstance> GI;
};
