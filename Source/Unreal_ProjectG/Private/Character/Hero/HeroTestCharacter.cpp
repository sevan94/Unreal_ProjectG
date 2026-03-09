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
#include "DataAssets/UI/EquipUIDataAsset.h"

void AHeroTestCharacter::BeginPlay()
{
    Super::BeginPlay();

    UPGGameInstance* GI = Cast<UPGGameInstance>(GetGameInstance());

    if (GI)
    {
        if (GI->CurrentWeapon) WeaponDataAsset = Cast<UDataAsset_WeaponData>(GI->CurrentWeapon->EquipDataAsset.LoadSynchronous());
        if (GI->CurrentArmor) ArmorDataAsset = Cast<UDataAsset_ArmorData>(GI->CurrentArmor->EquipDataAsset.LoadSynchronous());
        if (GI->CurrentAccessory) AccessoryDataAsset = Cast<UDataAsset_AccessoryData>(GI->CurrentArmor->EquipDataAsset.LoadSynchronous());
    }

    // 데이터 에셋 로드
    //WeaponDataAsset.LoadSynchronous();
    //ArmorDataAsset.LoadSynchronous();
    //AccessoryDataAsset.LoadSynchronous();

    if (WeaponDataAsset.IsValid()) HeroCombatComponent->EquipHeroWeapon(WeaponDataAsset.Get());
    if (ArmorDataAsset.IsValid()) HeroCombatComponent->EquipHeroArmor(ArmorDataAsset.Get());
    if (AccessoryDataAsset.IsValid()) HeroCombatComponent->EquipHeroAccessory(AccessoryDataAsset.Get());

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
