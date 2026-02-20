// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UnitPanelWidget.generated.h"

class UUnitUIDataAsset;
class UUnitSlotWidget;

/**
 * 
 */
UCLASS()
class UNREAL_PROJECTG_API UUnitPanelWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
    virtual void NativeConstruct() override;

public:
    // 에디터에서 5개의 유닛 데이터를 넣을 수 있는 배열
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BattelUI")
    TArray<TObjectPtr<UUnitUIDataAsset>> UnitDataList;

protected:
    // 유닛 슬롯 위젯
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UUnitSlotWidget> UnitSlot1;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UUnitSlotWidget> UnitSlot2;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UUnitSlotWidget> UnitSlot3;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UUnitSlotWidget> UnitSlot4;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UUnitSlotWidget> UnitSlot5;

private:
    // 슬롯들을 배열로 관리하기 위한 편의용 포인터 배열
    TArray<TObjectPtr<UUnitSlotWidget>> SlotArray;
};
