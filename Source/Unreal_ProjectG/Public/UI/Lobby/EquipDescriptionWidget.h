// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EquipDescriptionWidget.generated.h"

class UImage;
class UTextBlock;
class UEquipUIDataAsset;

/**
 * 
 */
UCLASS()
class UNREAL_PROJECTG_API UEquipDescriptionWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
    void UpdateDescription(UEquipUIDataAsset* InData);

protected:
    // 스킬 이미지
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UImage> SkillImage1;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UImage> SkillImage2;

    // 스킬 설명
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> SkillDescription1;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> SkillDescription2;
};
