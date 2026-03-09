// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "UnitData.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_PROJECTG_API UUnitData : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
    // --- [1] 기본 정보 ---
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Info")
    FGameplayTag ElementTag;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Info")
    FGameplayTag TeamTag;

    UPROPERTY(EditAnywhere, Category = "Stats")
    UDataAsset* BranchData;

    // --- [2] 외형 ---
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Visual")
    USkeletalMesh* SkeletalMesh;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Visual")
    TSubclassOf<UAnimInstance> AnimBlueprint;
};
