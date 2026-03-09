// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LobbyUnitWidget.generated.h"

class UButton;
class UHorizontalBox;
class UWidgetSwitcher;
class UPGGameInstance;
class UUnitListWidget;
class UUnitDescriptionWidget; 
class UUnitUIDataAsset;
class UUnitEntryObject;
class UPartyUnitWidget;

/**
 * 
 */
UCLASS()
class UNREAL_PROJECTG_API ULobbyUnitWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
    virtual void NativeConstruct() override;

    // 파티 슬롯 클릭 시 호출될 함수
    UFUNCTION()
    void HandlePartySlotClick(int32 SlotIndex);

    void InitializePartySlots();

private:
    UFUNCTION()
    void OnExitButtonClick();

    // 유닛 선택 시 데이터 저장
    UFUNCTION()
    void HandleUnitSelected(UUnitEntryObject* InData);

public:
    // 위젯 스위처
    UPROPERTY(BlueprintReadOnly, Category = "UI")
    TObjectPtr<UWidgetSwitcher> WidgetSwitcher;

    // 유닛 목록 위젯
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UUnitListWidget> UnitList;

    // 유닛 설명 위젯
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UUnitDescriptionWidget> UnitDescription;

    // 나가기 버튼
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> ExitButton;

    // 파티 슬롯 박스
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UHorizontalBox> PartyBox;

private:
    // 현재 리스트에서 선택된 유닛
    UPROPERTY()
    TObjectPtr<UUnitUIDataAsset> SelectedUnit;

    // 파티 슬롯 배열
    UPROPERTY()
    TArray<TObjectPtr<UPartyUnitWidget>> PartySlots;

    // 게임 인스턴스
    TObjectPtr<UPGGameInstance> GI;
};
