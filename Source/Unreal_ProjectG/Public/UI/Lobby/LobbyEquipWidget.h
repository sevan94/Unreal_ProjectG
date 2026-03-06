// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Types/PGEnumTypes.h"
#include "LobbyEquipWidget.generated.h"

class UButton;
class UImage;
class UWidgetSwitcher;
class UCurrentEquipWidget;
class UEquipListWidget;
class UEquipUIDataAsset;
class UEquipDescriptionWidget;

/**
 * 
 */
UCLASS()
class UNREAL_PROJECTG_API ULobbyEquipWidget : public UUserWidget
{
	GENERATED_BODY()

public:
    UFUNCTION()
    void SetEquipList(EEquipCategory InType);
	
protected:
    virtual void NativeConstruct() override;

private:
    UFUNCTION()
    void OnExitButtonClick();

    UFUNCTION()
    void OnEquipButtonClicked();

    // 장비 선택 시 데이터 저장
    UFUNCTION()
    void HandleEquipSelected(UEquipUIDataAsset* InData);

public:
    // 위젯 스위처
    UPROPERTY(BlueprintReadOnly, Category = "UI")
    TObjectPtr<UWidgetSwitcher> WidgetSwitcher;

protected:
    // 나가기 버튼
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> ExitButton;

    // 장비 장착 버튼
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> EquipButton;

    // 영웅 패널
    // 영웅 렌더 타깃
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UImage> HeroRender;

    // 장착 장비 패널
    // 장착 무기
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UCurrentEquipWidget> WeaponEquip;
    // 장착 방어구
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UCurrentEquipWidget> ArmorEquip;
    // 장착 장신구
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UCurrentEquipWidget> AccesoryEquip;

    // 장비 리스트 위젯
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UEquipListWidget> EquipList;

    // 장비 정보 위젯
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UEquipDescriptionWidget> EquipDescription;

private:
    // 현재 선택된 카테고리
    EEquipCategory CurrentActiveCategory;

    // 현재 리스트에서 선택된 장비
    UPROPERTY()
    TObjectPtr<UEquipUIDataAsset> SelectedEquip;
};
