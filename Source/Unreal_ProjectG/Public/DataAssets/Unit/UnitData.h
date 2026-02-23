// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "Types/PGStructTypes.h"
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

    //UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Visual")
    //UAnimMontage* AttackMontage;

    //// --- [3] 능력치 (GAS 초기화용) ---
    //UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stats")
    //float Health = 100.0f;

    //UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stats")
    //float AttackDamage = 10.0f;

    //UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stats")
    //float MoveSpeed = 300.0f;
};
