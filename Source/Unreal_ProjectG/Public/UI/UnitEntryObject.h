// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "UnitEntryObject.generated.h"

class UUnitUIDataAsset;

// 유닛 선택 시 호출
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUnitRequestSelected, UUnitEntryObject*, SelectedData);
// 유닛 데이터 변경 시 호출
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUnitDataChanged);
/**
 * 
 */
UCLASS()
class UNREAL_PROJECTG_API UUnitEntryObject : public UObject
{
	GENERATED_BODY()
	
public:
    // 버튼 클릭 시 호출할 함수
    UFUNCTION()
    void HandleClick() { OnUnitRequestSelected.Broadcast(this); }
    
    // 데이터 변경 시 호출할 함수
    UFUNCTION()
    void BroadcastDataChanged() { OnUnitDataChanged.Broadcast(); }

    void SetUnitUIData(UUnitUIDataAsset* InData) { UnitUIData = InData; }
    void SetIsOwned(bool bInOwned) { bIsOwned = bInOwned; }

    UFUNCTION(BlueprintCallable, Category = "Unit")
    UUnitUIDataAsset* GetUnitUIData() const { return UnitUIData; }
    UFUNCTION(BlueprintCallable, Category = "Unit")
    bool IsOwned() const { return bIsOwned; }

public:
    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnUnitRequestSelected OnUnitRequestSelected;
    
    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnUnitDataChanged OnUnitDataChanged;

private:
    // 유닛 데이터
    UPROPERTY()
    TObjectPtr<UUnitUIDataAsset> UnitUIData;

    // 유닛 소유 여부 상태
    UPROPERTY()
    bool bIsOwned = true;
};
