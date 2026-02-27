// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "DataAsset_SetBonusData.generated.h"

class UPGGameplayAbility;
/**
 * 
 */
UCLASS()
class UNREAL_PROJECTG_API UDataAsset_SetBonusData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
    // 세트 구분을 위한 태그
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (Categories = "Set"))
    FGameplayTag SetTag;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TSubclassOf<UPGGameplayAbility> BaseAbility; // 세트 보너스가 적용될 때, 제거되어야 하는 어빌리티

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TSubclassOf<UPGGameplayAbility> UpgradedAbility; // 세트 보너스 적용 후 부여되어야하는 어빌리티
};
