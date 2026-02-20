// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Character/Unit/UnitCharacter.h"
#include "UnitUIDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_PROJECTG_API UUnitUIDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
    // 유닛 캐릭터 블루프린트
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Unit")
    TSubclassOf<AUnitCharacter> UnitClass;

    // 유닛 버튼 일반 이미지
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Unit")
    TObjectPtr<UTexture2D> UnitButtonNormalImage;

    // 유닛 버튼 눌림 이미지
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Unit")
    TObjectPtr<UTexture2D> UnitButtonPressedImage;

    // 유닛 버튼 비활성화 이미지
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Unit")
    TObjectPtr<UTexture2D> UnitButtonUnableImage;

    // 유닛 코스트
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Unit")
    int32 UnitCost;
};
