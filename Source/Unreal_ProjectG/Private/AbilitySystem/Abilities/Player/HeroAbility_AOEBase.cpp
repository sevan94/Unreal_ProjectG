// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Player/HeroAbility_AOEBase.h"
#include "DataAssets/Ability/DataAsset_SkillData.h"
#include "PGGameplayTags.h"
#include "Actors/AOESkillActor.h"
#include "Actors/AOEActor/AOEInstantEffectActor.h"
#include "Actors/AOEActor/AOEDurationEffectActor.h"

UHeroAbility_AOEBase::UHeroAbility_AOEBase()
{
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UHeroAbility_AOEBase::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
    Super::OnGiveAbility(ActorInfo, Spec);

    // DataAsset_SkillData에서 AOECommonConfig 캐싱
    UDataAsset_SkillData* DataAsset = Cast<UDataAsset_SkillData>(Spec.SourceObject.Get());

    if (DataAsset)
    {
        //알맞은 Entry를 가져온다.
        const FAbilityEntry& Entry = DataAsset->GetGivenAbilityEntryForASC(GetAbilitySystemComponentFromActorInfo());
    
        const FHeroAOECommonConfig* Config = Entry.AbilityConfig.GetPtr<FHeroAOECommonConfig>();

        if(Config)
        {
            AOEConfig = *Config;
        }
    }

    //SoftPtr 로드
    AOEConfig.Montage = AOEConfig.Montage.LoadSynchronous();
}

void UHeroAbility_AOEBase::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);   
}

AAOESkillActor* UHeroAbility_AOEBase::SpawnAndInitializeAOEActor(const FVector& SpawnLocation, const FRotator& SpawnRotation)
{
    if (!AOEConfig.SpawnedActorClass)
    {
        UE_LOG(LogTemp, Warning, TEXT("AOEConfig에 SpawnedActorClass가 설정되지 않았습니다"));
        return nullptr;
    }

    UWorld* World = GetWorld();
    if (!World) return nullptr;

    // AOE 액터 스폰
    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = GetAvatarActorFromActorInfo();
    SpawnParams.Instigator = Cast<APawn>(GetAvatarActorFromActorInfo());
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    const FTransform SpawnTransform(SpawnRotation, SpawnLocation);

    AAOESkillActor* SpawnedAOEActor = World->SpawnActorDeferred<AAOESkillActor>(AOEConfig.SpawnedActorClass, SpawnTransform, SpawnParams.Owner, SpawnParams.Instigator, SpawnParams.SpawnCollisionHandlingOverride);

    if(!SpawnedAOEActor)
    {
        UE_LOG(LogTemp, Warning, TEXT("AOE 액터 스폰에 실패했습니다. SpawnedActorClass : %s"), *GetNameSafe(AOEConfig.SpawnedActorClass));
        return nullptr;
    }

    // Actor 타입에 따라 알맞은 EffectClass 선택
    FGameplayEffectSpecHandle SpecHandle;
    if(SpawnedAOEActor->IsA<AAOEInstantEffectActor>())
    {
        // InstantEffectActor에 대한 처리
        SpecHandle = AOEConfig.InstantEffectClass
            ? MakeOutgoingEffectSpec(AOEConfig.InstantEffectClass)
            : FGameplayEffectSpecHandle();
    }
    else if(SpawnedAOEActor->IsA<AAOEDurationEffectActor>())
    {
        // DurationEffectActor에 대한 처리
        SpecHandle = AOEConfig.BuffDebuffClass
            ? MakeOutgoingEffectSpec(AOEConfig.BuffDebuffClass)
            : FGameplayEffectSpecHandle();
    }
    SpawnedAOEActor->InitializeAOEActor(
        GetAvatarActorFromActorInfo(),
        AOEConfig.TargetPolicy,
        SpecHandle,
        GetAbilityLevel()
    );
    SpawnedAOEActor->FinishSpawning(SpawnTransform);

    return SpawnedAOEActor;
}