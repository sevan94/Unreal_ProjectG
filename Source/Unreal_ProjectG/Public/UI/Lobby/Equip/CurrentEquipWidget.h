// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Types/PGEnumTypes.h"
#include "CurrentEquipWidget.generated.h"

class UButton;
class UImage;
class UEquipUIDataAsset;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCurrentEquipSelected, EEquipCategory, Category);
/**
 * 
 */
UCLASS()
class UNREAL_PROJECTG_API UCurrentEquipWidget : public UUserWidget
{
	GENERATED_BODY()

public:
    // 아이템 데이터를 위젯에 세팅하는 함수
    UFUNCTION(BlueprintCallable, Category = "Equip")
    void UpdateEquipSlot(UEquipUIDataAsset* InData);

protected:
    virtual void NativeConstruct() override;

    // 버튼 클릭 시 호출될 함수
    UFUNCTION()
    void OnEquipButtonClicked();

public:
    // 에디터에서 어떤 카테고리인지 지정
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equip")
    EEquipCategory SlotCategory;

    UPROPERTY(BlueprintAssignable, Category = "Equip")
    FOnCurrentEquipSelected OnSelected;
	
protected:
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> EquipButton;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UImage> EquipImage;

};
