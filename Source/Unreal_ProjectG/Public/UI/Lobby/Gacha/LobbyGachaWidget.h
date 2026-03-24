// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Types/PGEnumTypes.h"
#include "LobbyGachaWidget.generated.h"

class UButton;
class UWidgetSwitcher;
class UGoodsBarWidget;
class UPGGameInstance;

/**
 * 
 */
UCLASS()
class UNREAL_PROJECTG_API ULobbyGachaWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
    virtual void NativeConstruct() override;

    UFUNCTION()
    void OnExitButtonClick();
    UFUNCTION()
    void OnUnitGachaClick();
    UFUNCTION()
    void OnEquipGachaClick();

    UFUNCTION()
    void UpdateGoodsBar(EGoodsCategory InCategory, int32 InValue);

public:
    UPROPERTY(BlueprintReadOnly, Category = "UI")
    TObjectPtr<UWidgetSwitcher> WidgetSwitcher;

protected:
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> ExitButton;

    // 소유 재화
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UGoodsBarWidget> Gem;
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UGoodsBarWidget> Unlock;

    // 뽑기 버튼
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> UnitGachaButton;
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> EquipGachaButton;


    // 뽑기 소모 비용 위젯
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UGoodsBarWidget> UnitGachaCost;
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UGoodsBarWidget> EquipGachaCost;

private:
    TObjectPtr<UPGGameInstance> GI;
};
