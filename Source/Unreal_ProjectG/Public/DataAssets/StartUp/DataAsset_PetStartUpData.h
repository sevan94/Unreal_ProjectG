// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataAssets/StartUp/DataAsset_StartupDataBase.h"
#include "DataAsset_PetStartUpData.generated.h"

class UDataAsset_SkillData;
/**
 * 
 */
UCLASS()
class UNREAL_PROJECTG_API UDataAsset_PetStartUpData : public UDataAsset_StartupDataBase
{
	GENERATED_BODY()
	
public:
    virtual void GiveToAbilitySystemComponent(UPGAbilitySystemComponent* InASCToGive, int32 InLevel = 1) override;

private:
    // 유닛만이 가지고 있는 어빌리티
    UPROPERTY(EditDefaultsOnly, Category = "StartupData")
    TArray<UDataAsset_SkillData*> UPetCombatAbility;
};
