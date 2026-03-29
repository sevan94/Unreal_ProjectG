// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Types/PGEnumTypes.h"
#include "LobbyEquipWidget.generated.h"

class UButton;
class UImage;
class UTextBlock;
class UWidgetSwitcher;
class UPGGameInstance;
class UHeroStatusWidget;
class UCurrentEquipWidget;
class UEquipListWidget;
class UEquipUIDataAsset;
class UEquipDescriptionWidget;
class UGoodsBarWidget;
class USetEffectWidget;

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

    void IntializeEquipSlots();

    UFUNCTION()
    void UpdateGoodsBar(EGoodsCategory InCategory, int32 InValue);

    // 현재 활성화된 세트를 업데이트하는 함수
    void UpdateSetEffects();

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
    // 장착 버튼 텍스트
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> EquipButtonText;

    // 해금 재화
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UGoodsBarWidget> Unlock;
    // 업그레이드 재화
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UGoodsBarWidget> Gold;

    // 영웅 패널
    // 영웅 렌더 타깃
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UImage> HeroRender;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UHeroStatusWidget> HeroStatus;

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

    // 해금 소모 재화
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UGoodsBarWidget> UnlockCostWidget;

    // 세트 효과 데이터 테이블
    UPROPERTY(EditAnywhere, Category = "Settings|Data")
    TObjectPtr<UDataTable> SetEffectDataTable;

    // 세트 효과 위젯
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<USetEffectWidget> SetEffectWidget;

private:
    // 현재 선택된 카테고리
    EEquipCategory CurrentActiveCategory;

    // 현재 리스트에서 선택된 장비
    UPROPERTY()
    TObjectPtr<UEquipUIDataAsset> SelectedEquip;

    // 게임 인스턴스
    TObjectPtr<UPGGameInstance> GI;
};
