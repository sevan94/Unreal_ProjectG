// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Pet/PetCharacter.h"
#include "DataAssets/StartUp/DataAsset_PetStartUpData.h"
#include "AbilitySystem/PGAbilitySystemComponent.h"
#include "Engine/AssetManager.h"

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
}
