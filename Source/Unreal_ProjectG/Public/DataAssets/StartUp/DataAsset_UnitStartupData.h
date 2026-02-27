// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataAssets/StartUp/DataAsset_StartupDataBase.h"
#include "GameplayTagContainer.h"
#include "Types/PGStructTypes.h"
#include "DataAsset_UnitStartupData.generated.h"

class UBranchDataAsset;
class UPGUnitGameplayAbility;

/**
 * 
 */
UCLASS()
class UNREAL_PROJECTG_API UDataAsset_UnitStartupData : public UDataAsset_StartupDataBase
{
	GENERATED_BODY()
	
public:
    virtual void GiveToAbilitySystemComponent(UPGAbilitySystemComponent* InASCToGive, int32 InLevel = 1) override;

    // --- [1] 기본 정보 ---
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Info")
    FGameplayTag ElementTag;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Info")
    FGameplayTag TeamTag;

    UPROPERTY(EditAnywhere, Category = "Stats")
    UBranchDataAsset* BranchData;

private:
    UPROPERTY(EditDefaultsOnly, Category = "StartupData")
    TArray<FAbilityEntry> UnitCombatAbilityEntries;
};
