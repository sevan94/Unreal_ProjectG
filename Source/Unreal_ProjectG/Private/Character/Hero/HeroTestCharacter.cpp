// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Hero/HeroTestCharacter.h"
#include "DataAssets/Items/DataAsset_WeaponData.h"
#include "DataAssets/Items/DataAsset_ArmorData.h"
#include "DataAssets/Items/DataAsset_AccessoryData.h"
#include "AnimInstance/Hero/PGHeroLinkedAnimLayer.h"
#include "AbilitySystem/PGAbilitySystemComponent.h"
#include "Components/Equipment/EquipmentsStorageComponent.h"
#include "Engine/AssetManager.h"
#include "AbilitySystem/Abilities/PGHeroGameplayAbility.h"

void AHeroTestCharacter::BeginPlay()
{
    Super::BeginPlay();

    // 데이터 에셋 로드
    WeaponDataAsset.LoadSynchronous();
    ArmorDataAsset.LoadSynchronous();
    AccessoryDataAsset.LoadSynchronous();

    EquipmentsStorageComponent->EquipHeroWeapon(WeaponDataAsset.Get());
    EquipmentsStorageComponent->EquipHeroArmor(ArmorDataAsset.Get());
    EquipmentsStorageComponent->EquipHeroAccessory(AccessoryDataAsset.Get());
}