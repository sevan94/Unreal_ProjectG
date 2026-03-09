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
#include "Mode/Save/PGGameInstance.h"
#include "DataAssets/UI/EquipUIDataAsset.h"
#include "Components/Combat/HeroCombatComponent.h"

void AHeroTestCharacter::BeginPlay()
{
    Super::BeginPlay();

    UPGGameInstance* GI = Cast<UPGGameInstance>(GetGameInstance());

    if (GI)
    {
        if (GI->CurrentWeapon) WeaponDataAsset = Cast<UDataAsset_WeaponData>(GI->CurrentWeapon->EquipDataAsset.LoadSynchronous());
        if (GI->CurrentArmor) ArmorDataAsset = Cast<UDataAsset_ArmorData>(GI->CurrentArmor->EquipDataAsset.LoadSynchronous());
        if (GI->CurrentAccessory) AccessoryDataAsset = Cast<UDataAsset_AccessoryData>(GI->CurrentAccessory->EquipDataAsset.LoadSynchronous());
    }

    if (WeaponDataAsset.IsValid()) EquipmentsStorageComponent->EquipHeroWeapon(WeaponDataAsset.Get());
    if (ArmorDataAsset.IsValid()) EquipmentsStorageComponent->EquipHeroArmor(ArmorDataAsset.Get());
    if (AccessoryDataAsset.IsValid()) EquipmentsStorageComponent->EquipHeroAccessory(AccessoryDataAsset.Get());

    // 인스턴스 멤버 함수로 호출하도록 수정
    if (HeroCombatComponent)
    {
        HeroCombatComponent->SetCombatMode(EHeroCombatMode::Manual);
    }
}