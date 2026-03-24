// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "EquipUIDataAsset.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class UNREAL_PROJECTG_API UEquipUIDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
    // 장비 구분용 ID
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Unit|Data")
    int32 EquipID;

    // 장비 데이터 에셋
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Equip")
    TSoftObjectPtr<UPrimaryDataAsset> EquipDataAsset;

    // 장비 버튼 이미지
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Equip")
    TObjectPtr<UTexture2D> EquipImage;

    // 장비 이름
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Equip")
    FName EquipName;

    // 스킬 1
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Equip")
    TObjectPtr<UTexture2D> SkillImage1;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Equip")
    FString SkillDescription1;

    // 스킬 2
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Equip")
    TObjectPtr<UTexture2D> SkillImage2;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Equip")
    FString SkillDescription2;
};
