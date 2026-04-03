// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "DataAsset_AccessoryData.generated.h"

class UPGHeroSkillGameplayAbility;
class UDataAsset_HeroSkillData;
class UGameplayAbility;
/**
 * 
 */
UCLASS()
class UNREAL_PROJECTG_API UDataAsset_AccessoryData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
    //FORCEINLINE const TSubclassOf<UPGGameplayAbility>& GetGrantedAbility() const { return AbilityToGrant; }
    bool IsValid() const;

public:
    // 세트 구분을 위한 태그
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (Categories = "Equipment.Set"))
    FGameplayTag SetTag;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability", meta = (DisplayName = "히어로 스킬 기반 어빌리티 클래스(어빌리티 데이터 필요)"))
    TSubclassOf<UPGHeroSkillGameplayAbility> AccessoryAbilityClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability", meta = (DisplayName = "히어로 스킬 기반 어빌리티 데이터"))
    TSoftObjectPtr<UDataAsset_HeroSkillData> AccessoryAbilityData;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability", meta = (DisplayName = "히어로 스킬 기반 어빌리티 클래스"))
    TSoftClassPtr<UGameplayAbility> AccessoryAbility;
};
