// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UISwitcherWidget.generated.h"

class UWidgetSwitcher;
class UMainLobbyWidget;
class ULobbyUnitWidget;
class ULobbyEquipWidget;
class ULobbyStageWidget;
class ULobbyGachaWidget;
class UGachaResultWidget;
class UAudioComponent;

/**
 * 
 */
UCLASS()
class UNREAL_PROJECTG_API UUISwitcherWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
    virtual void NativeConstruct() override;
	
public:
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    TObjectPtr<UWidgetSwitcher> WidgetSwitcher;

    // 로비 배경음
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TObjectPtr<USoundBase> LobbyBGM;

protected:
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UMainLobbyWidget> LobbyWidget;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<ULobbyUnitWidget> UnitWidget;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<ULobbyEquipWidget> EquipWidget;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<ULobbyStageWidget> StageWidget;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<ULobbyGachaWidget> GachaWidget;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UGachaResultWidget> GachaResultWidget;

    UPROPERTY()
    TObjectPtr<UAudioComponent> AudioComponent;
};
