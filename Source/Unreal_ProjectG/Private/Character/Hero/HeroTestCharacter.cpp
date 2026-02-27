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

AHeroTestCharacter::AHeroTestCharacter()
{

}

void AHeroTestCharacter::BeginPlay()
{
    Super::BeginPlay();

    SetupEquipmentToPawn();
}

void AHeroTestCharacter::SetupEquipmentToPawn()
{
    UAssetManager::GetStreamableManager().RequestAsyncLoad(
        WeaponDataAsset.ToSoftObjectPath(),
        FStreamableDelegate::CreateLambda(
            [this]()
            {
                if (UDataAsset_WeaponData* LoadedData = WeaponDataAsset.Get())
                {
                    SetupWeaponToPawn();
                }
            }
        )
    );

    // 비동기적으로 로드
    UAssetManager::GetStreamableManager().RequestAsyncLoad(
        ArmorDataAsset.ToSoftObjectPath(),
        FStreamableDelegate::CreateLambda(
            [this]()
            {
                if (UDataAsset_ArmorData* LoadedData = ArmorDataAsset.Get())
                {
                    SetupArmorToPawn();
                }
            }
        )
    );

    // 동기 로드
    if(AccessoryDataAsset.LoadSynchronous())
    {
        SetupAccessoryToPawn();
    }
}

void AHeroTestCharacter::SetupWeaponToPawn()
{
    UAssetManager::GetStreamableManager().RequestAsyncLoad(
        WeaponDataAsset->GetHeroWeaponData().SoftWeaponMesh.ToSoftObjectPath(),
        FStreamableDelegate::CreateLambda(
            [this]()
            {
                FGameplayAbilitySpecHandle BasicAttackAbilitySpecHandle;
                TArray<FGameplayAbilitySpecHandle> SkillAbilitySpecHandles;

                // 무기 메시 로드 성공 & 메쉬 설정
                WeaponStaticMesh->SetStaticMesh(WeaponDataAsset->GetHeroWeaponData().SoftWeaponMesh.Get());
                GetMesh()->LinkAnimClassLayers(WeaponDataAsset->GetHeroWeaponData().WeaponAnimLayer);

                // 무기 기본 공격 어빌리티 부여
                PGAbilitySystemComponent->GrantHeroWeaponBasicAttackAbility(
                    WeaponDataAsset->GetHeroWeaponData().WeaponBasicAttackAbilityEntry,
                    TestAbilityLevel,
                    BasicAttackAbilitySpecHandle
                );

                // 무기 스킬 어빌리티들 부여
                PGAbilitySystemComponent->GrantHeroWeaponSkillAbilities(
                    WeaponDataAsset->GetHeroWeaponData().WeaponSkillAbilityEntries,
                    TestAbilityLevel,
                    SkillAbilitySpecHandles
                );


                // 부여한 어빌리티 핸들을 컴뱃 컴포넌트에 설정
                HeroCombatComponent->AssignBaseAttackAbilitySpecHandle(BasicAttackAbilitySpecHandle);
                HeroCombatComponent->AssignSkillAbilitySpecHandle(SkillAbilitySpecHandles);
                HeroCombatComponent->SetbWeaponEquipped(true);
                WeaponStaticMesh->AttachToComponent(
                    GetMesh(),
                    FAttachmentTransformRules::SnapToTargetNotIncludingScale,
                    WeaponDataAsset->GetHeroWeaponData().WeaponSocketName
                );
                WeaponStaticMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
                WeaponStaticMesh->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));
            }
        )
    );
}

void AHeroTestCharacter::SetupArmorToPawn()
{
    ArmorDataAsset.Get()->MakeOutgoingArmorEffectSpecHandle(PGAbilitySystemComponent, TestAbilityLevel);
}

void AHeroTestCharacter::SetupAccessoryToPawn()
{
    FGameplayAbilitySpec AbilitySpec(AccessoryDataAsset.Get()->GetGrantedAbility());
    AbilitySpec.SourceObject = this;
    AbilitySpec.Level = TestAbilityLevel;
    PGAbilitySystemComponent->GiveAbility(AbilitySpec);

    PGAbilitySystemComponent->TryActivateAbility(AbilitySpec.Handle);
}
