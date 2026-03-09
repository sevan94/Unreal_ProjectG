// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Types/PGEnumTypes.h"
#include "EquipListWidget.generated.h"

class UDataTable;
class UTileView;
class UEquipEntryObject;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEquipSelected, UEquipUIDataAsset*, SelectedData);
/**
 * 
 */
UCLASS()
class UNREAL_PROJECTG_API UEquipListWidget : public UUserWidget
{
	GENERATED_BODY()
    
public:
    // 타입에 맞춰 리스트 갱신
    UFUNCTION(BlueprintCallable, Category = "Equip")
    void UpdateList(EEquipCategory InCategory);

    void ClearTileView();

protected:
    virtual void NativeConstruct() override;
	
    // 타일뷰의 아이템 클릭 시
    UFUNCTION()
    void OnItemClicked(UEquipUIDataAsset* SelectedData);

public:
    UPROPERTY(BlueprintAssignable, Category = "Equip")
    FOnEquipSelected OnEquipSelected;

protected:
    // 타일뷰 바인딩
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTileView> EquipTileView;

    // 타입별 장비 데이터 테이블
    UPROPERTY(EditAnywhere, Category = "Equip")
    TMap<EEquipCategory, TObjectPtr<UDataTable>> EquipDataTables;

};
