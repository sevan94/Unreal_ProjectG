// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UnitSlotWidget.generated.h"

class UUnitUIDataAsset;
class UButton;
class UTextBlock;

/**
 * 
 */
UCLASS()
class UNREAL_PROJECTG_API UUnitSlotWidget : public UUserWidget
{
	GENERATED_BODY()

public:
    // 데이터 에셋을 가져와서 초기화
    UFUNCTION(BlueprintCallable, Category = "BattelUI")
    void InitializeSlot(UUnitUIDataAsset* InDataAsset);

protected:
    // 초기화 및 버튼 이벤트 바인딩
    virtual void NativeConstruct() override;

    // 버튼 클릭 시 실행될 함수
    UFUNCTION()
    void OnUnitButtonClicked();

protected:
    UPROPERTY(BlueprintReadOnly, Category = "BattelUI")
    TObjectPtr<UUnitUIDataAsset> UnitData;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> UnitButton;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> UnitCost;
};
