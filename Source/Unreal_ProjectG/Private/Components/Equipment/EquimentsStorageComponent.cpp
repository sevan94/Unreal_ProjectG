// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Equipment/EquipmentsStorageComponent.h"
#include "DataAssets/Items/DataAsset_WeaponData.h"
#include "DataAssets/Items/DataAsset_ArmorData.h"
#include "DataAssets/Items/DataAsset_AccessoryData.h"
#include "DataAssets/Ability/DataAsset_SkillData.h"
#include "AbilitySystem/PGAbilitySystemComponent.h"
#include "AbilitySystem/Abilities/PGHeroGameplayAbility.h"
#include "Character/Hero/HeroCharacter.h"
#include "AnimInstance/Hero/PGHeroLinkedAnimLayer.h"
#include "AbilitySystem/PGCharacterAttributeSet.h"
#include "PGFunctionLibrary.h"

void UEquipmentsStorageComponent::Startup(UDataAsset_WeaponData* InWeaponData, UDataAsset_ArmorData* InArmorData, UDataAsset_AccessoryData* InAccessoryData)
{
    EquipHeroWeapon(InWeaponData);
    EquipHeroArmor(InArmorData);
    EquipHeroAccessory(InAccessoryData);
}

void UEquipmentsStorageComponent::EquipHeroWeapon(UDataAsset_WeaponData* InWeaponData)
{
    UPGAbilitySystemComponent* ASC = UPGFunctionLibrary::NativeGetPGASCFromActor(GetOwner());

    // 무기 메시 장착 및 캐싱 로직
    AHeroCharacter* HeroCharacter = Cast<AHeroCharacter>(GetOwner());
    if (HeroCharacter)
    {
        CachedWeaponMeshComponent = HeroCharacter->GetWeaponStaticMesh();
    }
    if (HeroCharacter && CachedWeaponMeshComponent.IsValid())
    {
        // 무기에 메시를 히어로에 장착
        CachedWeaponMeshComponent->AttachToComponent(HeroCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, InWeaponData->WeaponSocketName);
        InWeaponData->SoftWeaponMesh.LoadSynchronous();
        CachedWeaponMeshComponent->SetStaticMesh(InWeaponData->SoftWeaponMesh.Get());

        // 애니메이션 레이어 링크 세팅
        InWeaponData->WeaponAnimLayer.LoadSynchronous();
        HeroCharacter->GetMesh()->LinkAnimClassLayers(InWeaponData->WeaponAnimLayer.Get());

        CachedWeaponMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
        CachedWeaponMeshComponent->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));

        bWeaponEquipped = true;
    }

    if (ASC)
    {
        // 무기 어빌리티 부여(핸들 반환 필요)
        InWeaponData->GrantWeaponAbilityToAbilityComponent(ASC, 1, GrantedWeaponAbilitySpecHandles);

        // 무기의 공격속도와 공격 사거리를 ASC의 어트리뷰트에 세팅
        ASC->SetNumericAttributeBase(UPGCharacterAttributeSet::GetAttackSpeedAttribute(), InWeaponData->WeaponAttackSpeed);
        ASC->SetNumericAttributeBase(UPGCharacterAttributeSet::GetAttackRangeAttribute(), InWeaponData->WeaponAttackRange);

        if (OnWeaponAbilitiesActivate.IsBound())
            OnWeaponAbilitiesActivate.Broadcast();
    }
}

void UEquipmentsStorageComponent::EquipHeroArmor(UDataAsset_ArmorData* InArmorData)
{
    UPGAbilitySystemComponent* ASC = UPGFunctionLibrary::NativeGetPGASCFromActor(GetOwner());
    InArmorData->MakeOutgoingArmorEffectSpecHandle(ASC, 1);
}

void UEquipmentsStorageComponent::EquipHeroAccessory(UDataAsset_AccessoryData* InAccessoryData)
{
    UPGAbilitySystemComponent* ASC = UPGFunctionLibrary::NativeGetPGASCFromActor(GetOwner());
    UDataAsset_SkillData* AccessoryAbilityAsset = InAccessoryData->AccessoryAbilityData.LoadSynchronous();
    AccessoryAbilityAsset->AbilityEntry.AbilityClass;

    FGameplayAbilitySpec AbilitySpec(
        AccessoryAbilityAsset->AbilityEntry.AbilityClass,
        1,
        INDEX_NONE,
        AccessoryAbilityAsset
    );
    ASC->GiveAbility(AbilitySpec);
    ASC->TryActivateAbility(AbilitySpec.Handle);
}