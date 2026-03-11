// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/DataTable/StageDataTable.h"
#include "StageMapWidget.generated.h"

class UCanvasPanel;
class UStageButtonWidget;

/**
 * 
 */
UCLASS()
class UNREAL_PROJECTG_API UStageMapWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
    // 외부에서 지도를 새로고침할 때 호출
    void RefreshStageMap();

protected:
    virtual void NativeConstruct() override;

private:
    void GenerateStageButtons();

protected:
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UCanvasPanel> MapContainer;

    UPROPERTY(EditAnywhere, Category = "Stage Settings")
    TSubclassOf<UStageButtonWidget> StageButtonClass;

    UPROPERTY(EditAnywhere, Category = "Stage Settings")
    TObjectPtr<UDataTable> StageDataTable;
};
