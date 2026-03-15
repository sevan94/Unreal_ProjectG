// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "UnitEntryObject.generated.h"

class UUnitUIDataAsset;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUnitRequestSelected, UUnitUIDataAsset*, SelectedData);
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
    void HandleClick() { OnUnitRequestSelected.Broadcast(UnitUIData); }

    void SetUnitUIData(UUnitUIDataAsset* InData) { UnitUIData = InData; }
    void SetIsOwned(bool bInOwned) { bIsOwned = bInOwned; }

    UFUNCTION(BlueprintCallable, Category = "Unit")
    UUnitUIDataAsset* GetUnitUIData() const { return UnitUIData; }
    UFUNCTION(BlueprintCallable, Category = "Unit")
    bool IsOwned() const { return bIsOwned; }

public:
    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnUnitRequestSelected OnUnitRequestSelected;

private:
    // 유닛 데이터
    UPROPERTY()
    TObjectPtr<UUnitUIDataAsset> UnitUIData;

    // 유닛 소유 여부 상태
    UPROPERTY()
    bool bIsOwned = true;
};
