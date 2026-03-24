// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Combat/PawnCombatComponent.h"
#include "HeroCombatComponent.generated.h"

class UDataAsset_WeaponData;
class UDataAsset_ArmorData;
class UDataAsset_AccessoryData;
class UDataAsset_SetBonusData;

/**
 * 
 */
UCLASS(Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UNREAL_PROJECTG_API UHeroCombatComponent : public UPawnCombatComponent
{
	GENERATED_BODY()
	
public:
    UFUNCTION(BlueprintCallable, Category = "Combat")
    void EquipHeroWeapon(UDataAsset_WeaponData* InWeaponData);

    UFUNCTION(BlueprintCallable, Category = "Combat")
    void EquipHeroArmor(UDataAsset_ArmorData* InArmorData);

    UFUNCTION(BlueprintCallable, Category = "Combat")
    void EquipHeroAccessory(UDataAsset_AccessoryData* InAccessoryData);

    UFUNCTION(BlueprintCallable, Category = "Combat")
    void ApplySetBonus(UDataAsset_SetBonusData* InSetBonusData);

public:
    TWeakObjectPtr<UStaticMeshComponent> CachedWeaponMeshComponent;
};
