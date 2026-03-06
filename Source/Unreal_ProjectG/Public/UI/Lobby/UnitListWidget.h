// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UnitListWidget.generated.h"

class UDataTable;
class UTileView;
class UUnitEntryObject;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUnitSelected, UUnitUIDataAsset*, SelectedData);
/**
 * 
 */
UCLASS()
class UNREAL_PROJECTG_API UUnitListWidget : public UUserWidget
{
	GENERATED_BODY()

public:
    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnUnitSelected OnUnitSelected;
	
protected:
    virtual void NativeConstruct() override;

    // Tile View 초기화
    UFUNCTION(BlueprintCallable, Category = "Unit")
    void InitializeUnitList();

    // 유닛 클릭시
    UFUNCTION()
    void OnUnitClicked(UUnitUIDataAsset* SelectedData);

protected:
    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Unit")
    TObjectPtr<UDataTable> UnitDataTable;

    UPROPERTY(meta = (BindWidget))
    TSoftObjectPtr<UTileView> UnitTileView;

    UPROPERTY(EditDefaultsOnly, Category = "Unit")
    TSubclassOf<UUnitEntryObject> EntryObjectClass;
};
