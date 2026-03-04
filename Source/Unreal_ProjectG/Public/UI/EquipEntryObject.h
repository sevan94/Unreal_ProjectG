// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "EquipEntryObject.generated.h"

class UEquipUIDataAsset;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEquipRequestSelected, UEquipUIDataAsset*, SelectedData);
/**
 * 
 */
UCLASS()
class UNREAL_PROJECTG_API UEquipEntryObject : public UObject
{
	GENERATED_BODY()
	
public:
    // 버튼 클릭 시 호출할 함수
    UFUNCTION()
    void HandleClick() { OnEquipRequestSelected.Broadcast(EquipUIData); }

    void SetEquipUIData(UEquipUIDataAsset* InData) { EquipUIData = InData; }
    void SetIsOwned(bool bInOwned) { bIsOwned = bInOwned; }

    UFUNCTION(BlueprintCallable, Category = "Unit")
    UEquipUIDataAsset* GetEquipUIData() const { return EquipUIData; }
    UFUNCTION(BlueprintCallable, Category = "Unit")
    bool IsOwned() const { return bIsOwned; }

public:
    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnEquipRequestSelected OnEquipRequestSelected;

private:
    // 장비 데이터
    UPROPERTY()
    TObjectPtr<UEquipUIDataAsset> EquipUIData;

    // 장비 소유 여부 상태
    UPROPERTY()
    bool bIsOwned = true;
};
