// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StageReadyWidget.generated.h"

class UButton;
class UReadyUnitWidget;
class UReadyEquipWidget;
class UPGGameInstance;

/**
 * 
 */
UCLASS()
class UNREAL_PROJECTG_API UStageReadyWidget : public UUserWidget
{
	GENERATED_BODY()

public:
    void InitializeReadyWidget();
	
protected:
    virtual void NativeConstruct() override;

    UFUNCTION()
    void OnExitButtonClicked();

    // 각종 초기화 함수
    void InitializeReadyUnit();
    void InitializeReadyEquip();

protected:
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> ExitButton;

    // 현재 유닛 패널
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UReadyUnitWidget> ReadyUnit1;
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UReadyUnitWidget> ReadyUnit2;
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UReadyUnitWidget> ReadyUnit3;
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UReadyUnitWidget> ReadyUnit4;
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UReadyUnitWidget> ReadyUnit5;

    // 장착 장비 패널
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UReadyEquipWidget> ReadyWeapon;
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UReadyEquipWidget> ReadyArmor;
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UReadyEquipWidget> ReadyAccessory;

    // 적 정보 패널


private:
    TObjectPtr<UPGGameInstance> GI;
};
