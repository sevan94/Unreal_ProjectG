// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Pet/PetCharacter.h"
#include "DataAssets/StartUp/DataAsset_PetStartUpData.h"
#include "AbilitySystem/PGAbilitySystemComponent.h"
#include "Engine/AssetManager.h"
#include "AbilitySystem/PGCharacterAttributeSet.h"

APetCharacter::APetCharacter()
{
    PrimaryActorTick.bCanEverTick = false;
}

void APetCharacter::PossessedBy(AController* NewController)
{
    Super::PossessedBy(NewController);

    if (CharacterStartupData.IsNull())
    {
        return;
    }

    UAssetManager::GetStreamableManager().RequestAsyncLoad(
        CharacterStartupData.ToSoftObjectPath(),
        FStreamableDelegate::CreateLambda(
            [this]()
            {
                if (UDataAsset_StartupDataBase* LoadedData = CharacterStartupData.Get())
                {
                    if (PGAbilitySystemComponent)
                    {
                        LoadedData->GiveToAbilitySystemComponent(PGAbilitySystemComponent);
                    }
                }
            }
        )
    );

    // 소유자 캐릭터의 공격력에 비례하여 펫의 공격력 설정
    if (GetInstigator())
    {
        float InstigatorAttackPower = 0.f;
        if (APGCharacterBase* InstigatorCharacter = Cast<APGCharacterBase>(GetInstigator()))
        {
            if (UPGCharacterAttributeSet* InstigatorAttributeSet = InstigatorCharacter->GetPGCharacterAttributeSet())
            {
                InstigatorAttackPower = InstigatorAttributeSet->GetAttackPower();
                PGAbilitySystemComponent->SetNumericAttributeBase(UPGCharacterAttributeSet::GetAttackPowerAttribute(), InstigatorAttackPower * AttackPowerMultiplier);
            }
        }
    }
}
