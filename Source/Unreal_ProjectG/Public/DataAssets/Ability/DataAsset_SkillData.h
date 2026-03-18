// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PGGameplayTags.h"
#include "Types/AbilityConfig.h"
#include "DataAsset_SkillData.generated.h"

class UPGGameplayAbility;
class UAbilitySystemComponent;
/**
 * 
 */
UCLASS()
class UNREAL_PROJECTG_API UDataAsset_SkillData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
   UFUNCTION(BlueprintCallable)
   const FAbilityEntry& GetGivenAbilityEntryForASC(const UAbilitySystemComponent* InASC) const;

public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (DisplayName = "Base Ability Entry"))
    FAbilityEntry AbilityEntry;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Upgraded", meta = (DisplayName = "Required Ability Tag", Categories = "Equipment.Set"))
    FGameplayTag UpgradeRequirementTag;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Upgraded", meta = (DisplayName = "Upgrade Ability Entry"))
    FAbilityEntry UpgradeAbilityEntry;
};
