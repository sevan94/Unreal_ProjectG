// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Combat/HeroCombatComponent.h"
#include "DataAssets/Items/DataAsset_WeaponData.h"
#include "DataAssets/Items/DataAsset_ArmorData.h"
#include "DataAssets/Items/DataAsset_AccessoryData.h"
#include "DataAssets/Items/DataAsset_SetBonusData.h"
#include "AbilitySystem/PGAbilitySystemComponent.h"
#include "GameFramework/Character.h"
#include "AnimInstance/Hero/PGHeroLinkedAnimLayer.h"
#include "AbilitySystem/Abilities/PGHeroGameplayAbility.h"
#include "DataAssets/Ability/DataAsset_SkillData.h"
#include "PGFunctionLibrary.h"

void UHeroCombatComponent::EquipHeroWeapon(UDataAsset_WeaponData* InWeaponData)
{
    UPGAbilitySystemComponent* ASC = UPGFunctionLibrary::NativeGetWarriorASCFromActor(GetOwner());

    // 무기 메시 장착 및 캐싱 로직
    USkeletalMeshComponent* HeroMesh = Cast<ACharacter>(GetOwner())->GetMesh();
    if (HeroMesh)
    {
        // 무기에 메시를 히어로에 장착
        UStaticMeshComponent* WeaponStaticMesh = NewObject<UStaticMeshComponent>(GetOwner());
        WeaponStaticMesh->RegisterComponent();
        WeaponStaticMesh->AttachToComponent(HeroMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, InWeaponData->GetHeroWeaponData().WeaponSocketName);
        InWeaponData->GetHeroWeaponData().SoftWeaponMesh.LoadSynchronous();
        WeaponStaticMesh->SetStaticMesh(InWeaponData->GetHeroWeaponData().SoftWeaponMesh.Get());

        // 애니메이션 레이어 링크 세팅
        HeroMesh->LinkAnimClassLayers(InWeaponData->GetHeroWeaponData().WeaponAnimLayer);

        WeaponStaticMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
        WeaponStaticMesh->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));

        CachedWeaponMeshComponent = WeaponStaticMesh;
        bWeaponEquipped = true;
    }

    if (ASC)
    {
        // 어빌리티를 Give하고 스펙 핸들을 저장하는 로직
        // 기본 공격 어빌리티 핸들 반환
        UDataAsset_SkillData* BasicAttackData = InWeaponData->GetHeroWeaponData().WeaponBasicAttackAbility.LoadSynchronous();
        if (BasicAttackData)
        {
            ASC->GrantHeroWeaponBasicAttackAbility(BasicAttackData->AbilityEntry, 1, GrantedBaseAttackAbilitySpecHandle);
        }
        
        TArray<FAbilityEntry> SkillAbilityEntries;
        SkillAbilityEntries.Empty();
        for (const TSoftObjectPtr<UDataAsset_SkillData>& SkillAbilityDataPtr : InWeaponData->GetHeroWeaponData().WeaponSkillAbilityList)
        {
            SkillAbilityDataPtr.LoadSynchronous();
            if (SkillAbilityDataPtr.IsValid())
            {
                UDataAsset_SkillData* SkillAbilityData = SkillAbilityDataPtr.Get();
                SkillAbilityEntries.Add(SkillAbilityData->AbilityEntry);
            }
        }
        ASC->GrantHeroWeaponSkillAbilities(SkillAbilityEntries, 1, GrantedSkillAbilitySpecHandles);

        if (OnWeaponAbilitiesActivate.IsBound())
        {
            OnWeaponAbilitiesActivate.Broadcast();
        }
    }
}

void UHeroCombatComponent::EquipHeroArmor(UDataAsset_ArmorData* InArmorData)
{
    UPGAbilitySystemComponent* ASC = UPGFunctionLibrary::NativeGetWarriorASCFromActor(GetOwner());
    InArmorData->MakeOutgoingArmorEffectSpecHandle(ASC, 1);
}

void UHeroCombatComponent::EquipHeroAccessory(UDataAsset_AccessoryData* InAccessoryData)
{
    UPGAbilitySystemComponent* ASC = UPGFunctionLibrary::NativeGetWarriorASCFromActor(GetOwner());

    FGameplayAbilitySpec AbilitySpec(InAccessoryData->GetGrantedAbility());
    AbilitySpec.SourceObject = this;
    ASC->GiveAbility(AbilitySpec);

    ASC->TryActivateAbility(AbilitySpec.Handle);
}

void UHeroCombatComponent::ApplySetBonus(UDataAsset_SetBonusData* InSetBonusData)
{

}
