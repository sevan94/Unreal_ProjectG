// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Hero/HeroTestCharacter.h"
#include "DataAssets/Items/DataAsset_WeaponData.h"
#include "DataAssets/Items/DataAsset_ArmorData.h"
#include "DataAssets/Items/DataAsset_AccessoryData.h"
#include "AnimInstance/Hero/PGHeroLinkedAnimLayer.h"
#include "AbilitySystem/PGAbilitySystemComponent.h"
#include "Components/Combat/HeroCombatComponent.h"
#include "Engine/AssetManager.h"
#include "AbilitySystem/Abilities/PGHeroGameplayAbility.h"
#include "Mode/Save/PGGameInstance.h"

void AHeroTestCharacter::BeginPlay()
{
    Super::BeginPlay();

    // 데이터 에셋 로드
    WeaponDataAsset.LoadSynchronous();
    ArmorDataAsset.LoadSynchronous();
    AccessoryDataAsset.LoadSynchronous();

    HeroCombatComponent->EquipHeroWeapon(WeaponDataAsset.Get());
    HeroCombatComponent->EquipHeroArmor(ArmorDataAsset.Get());
    HeroCombatComponent->EquipHeroAccessory(AccessoryDataAsset.Get());

}

//void AHeroTestCharacter::SetupAccessoryToPawn()
//{
//    FGameplayAbilitySpec AbilitySpec(AccessoryDataAsset.Get()->GetGrantedAbility());
//    AbilitySpec.SourceObject = this;
//    AbilitySpec.Level = TestAbilityLevel;
//    PGAbilitySystemComponent->GiveAbility(AbilitySpec);
//
//    PGAbilitySystemComponent->TryActivateAbility(AbilitySpec.Handle);
//}
//
