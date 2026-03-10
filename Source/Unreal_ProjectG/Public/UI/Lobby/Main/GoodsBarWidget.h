// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GoodsBarWidget.generated.h"

class UImage;
class UTextBlock;

/**
 * 
 */
UCLASS()
class UNREAL_PROJECTG_API UGoodsBarWidget : public UUserWidget
{
	GENERATED_BODY()

public:
    void InitializeGoodsBar(int32 InValue);

    UFUNCTION()
    void UpdateGoodsText(int32 InValue);

protected:
    virtual void NativeConstruct() override;
	
public:
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "UI|Lobby")
    TObjectPtr<UTexture2D> GoodsIcon;

protected:
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UImage> GoodsImage;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> GoodsText;
};
