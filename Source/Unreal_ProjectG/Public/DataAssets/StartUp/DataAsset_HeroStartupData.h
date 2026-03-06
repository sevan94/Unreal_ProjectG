// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataAssets/StartUp/DataAsset_StartupDataBase.h"
#include "DataAsset_HeroStartupData.generated.h"

class UPGHeroGameplayAbility;
/**
 * 
 */
UCLASS()
class UNREAL_PROJECTG_API UDataAsset_HeroStartupData : public UDataAsset_StartupDataBase
{
	GENERATED_BODY()
	
// 태그와 어빌리티 매핑이 필요해지면 사용
//public:
virtual void GiveToAbilitySystemComponent(UPGAbilitySystemComponent* InASCToGive, int32 InLevel) override;

private:
UPROPERTY(EditDefaultsOnly, Category = "HeroStartupData")
TArray<TSubclassOf<UPGHeroGameplayAbility>> HeroAbilities;
};
