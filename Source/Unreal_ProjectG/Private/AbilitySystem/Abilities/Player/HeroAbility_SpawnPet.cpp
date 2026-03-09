// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Player/HeroAbility_SpawnPet.h"
#include "DataAssets/Ability/DataAsset_SkillData.h"
#include "Kismet/GameplayStatics.h"
#include "Character/Pet/PetCharacter.h"
#include "Character/Hero/HeroCharacter.h"

void UHeroAbility_SpawnPet::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
    Super::OnGiveAbility(ActorInfo, Spec);

    UDataAsset_SkillData* DataAsset = Cast<UDataAsset_SkillData>(GetCurrentAbilitySpec()->SourceObject.Get());
    if (DataAsset)
    {
        const FAbilityEntry& SelectedAbilityEntry = DataAsset->GetGivenAbilityEntryForASC(GetAbilitySystemComponentFromActorInfo());
        const FHeroSpawnPetAbilityConfig* Config = SelectedAbilityEntry.AbilityConfig.GetPtr<FHeroSpawnPetAbilityConfig>();
        if (Config)
        {
            HeroSpawnPetConfig = *Config;
        }
    }
}

void UHeroAbility_SpawnPet::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    //==============================================
    // FHeroSpawnPetAbilityConfig의 SoftPtr 로드
    HeroSpawnPetConfig.SpawnedPetClass.LoadSynchronous();
    //==============================================

    if (HeroSpawnPetConfig.SpawnedPetClass.IsValid())
    {
        FVector SpawnLocation = GetHeroCharacterFromActorInfo()->GetActorLocation() + GetHeroCharacterFromActorInfo()->GetActorForwardVector() * -300.f; // 영웅 앞쪽에 소환
        FActorSpawnParameters SpawnParams;
        SpawnParams.Owner = GetHeroCharacterFromActorInfo();
        SpawnParams.Instigator = GetHeroCharacterFromActorInfo();
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
        
        APetCharacter* SpawnedPet = GetWorld()->SpawnActor<APetCharacter>(HeroSpawnPetConfig.SpawnedPetClass.Get(), FTransform(FRotator::ZeroRotator, SpawnLocation), SpawnParams);
    }
    EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

void UHeroAbility_SpawnPet::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
