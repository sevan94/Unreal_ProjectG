// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PGGameplayTags.h"
#include "Types/PGEnumTypes.h"
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

   // 일단은 UI 사용 용도로 Public하게 두었음, 추후 로직에 들어가면 위치 변경 가능
   // FAbilityEntry 안에 둔것이 아니기 때문에, 스킬 발동 방식은 Upgrade와 기본 스킬 모두 공통되어야 함. 추후, 생각 바뀌면 이동 가능
   UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (DisplayName = "스킬 발동 방식"))
   ESkillActivationType ActivationType = ESkillActivationType::Instant;

public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (DisplayName = "Base Ability Entry"))
    FAbilityEntry AbilityEntry;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Upgraded", meta = (DisplayName = "Required Ability Tag", Categories = "Equipment.Set"))
    FGameplayTag UpgradeRequirementTag;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Upgraded", meta = (DisplayName = "Upgrade Ability Entry"))
    FAbilityEntry UpgradeAbilityEntry;
};
