// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PartyUnitWidget.generated.h"

class UImage;
class UButton;
class UOverlay;
class UUnitUIDataAsset;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPartySlotClicked, int32, SlotIndex);
/**
 * 
 */
UCLASS()
class UNREAL_PROJECTG_API UPartyUnitWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
    // 외부(LobbyUnitWidget)에서 호출할 데이터 갱신 함수
    void UpdateSlot(UUnitUIDataAsset* NewData);

    // 교체 유도 오버레이 제어
    void ShowReplaceOverlay(bool bVisible);

    UUnitUIDataAsset* GetUnitData() const { return CurrentUnitData; }
protected:
    virtual void NativeConstruct() override;

    // 버튼 클릭 시 내부 처리 함수
    UFUNCTION()
    void OnInternalButtonClicked();

public:
    // 부모 위젯에서 구독할 클릭 이벤트
    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnPartySlotClicked OnSlotClicked;

    // 슬롯 식별 번호 (0~4)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slot Settings")
    int32 SlotIndex;

    // 현재 슬롯에 담긴 데이터
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data")
    TObjectPtr<UUnitUIDataAsset> CurrentUnitData;


protected:
    // 블루프린트 위젯 바인딩
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UImage> UnitImage;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> UnitButton;

    // 교체 오버레이
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UOverlay> ReplaceOverlay;
};
