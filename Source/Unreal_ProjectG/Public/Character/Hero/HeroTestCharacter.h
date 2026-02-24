// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Hero/HeroCharacter.h"
#include "HeroTestCharacter.generated.h"

class UDataAsset_WeaponData;
class UDataAsset_ArmorData;
class UDataAsset_AccessoryData;
/**
 * 어빌리티 테스트용 캐릭터,
 * 후에 삭제 예정
 */
UCLASS()
class UNREAL_PROJECTG_API AHeroTestCharacter : public AHeroCharacter
{
	GENERATED_BODY()

public:
    AHeroTestCharacter();

    //void SetupEquipmentToPawn();

protected:
    virtual void BeginPlay() override;

//private:
    //void SetupWeaponToPawn();
    //void SetupArmorToPawn();
    //void SetupAccessoryToPawn();  
private:
    UPROPERTY(EditDefaultsOnly, Category = "WeaponData")
    TSoftObjectPtr<UDataAsset_WeaponData> WeaponDataAsset;

    UPROPERTY(EditDefaultsOnly, Category = "ArmorData")
    TSoftObjectPtr<UDataAsset_ArmorData> ArmorDataAsset;

    UPROPERTY(EditDefaultsOnly, Category = "AccessoryData")
    TSoftObjectPtr<UDataAsset_AccessoryData> AccessoryDataAsset;

    UPROPERTY(EditDefaultsOnly, Category = "AbilityTest")
    int32 TestAbilityLevel = 1;
};
