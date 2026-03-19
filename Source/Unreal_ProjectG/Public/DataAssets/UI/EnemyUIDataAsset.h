// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Character/Unit/UnitCharacter.h"
#include "EnemyUIDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_PROJECTG_API UEnemyUIDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
    // 적 캐릭터 블루프린트
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy")
    TSubclassOf<AUnitCharacter> EnemyClass;

    // 적 이미지
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy")
    TObjectPtr<UTexture2D> EnemyImage;
};
