// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Player/HeroAbility_AOEBase.h"
#include "DataAssets/Ability/DataAsset_SkillData.h"
#include "PGGameplayTags.h"
#include "Actors/AOESkillActor.h"

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

AAOESkillActor* UHeroAbility_AOEBase::SpawnAndInitializeAOEActor(const FVector& SpawnLocation)
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

    const FTransform SpawnTransform(FRotator::ZeroRotator, SpawnLocation);

    AAOESkillActor* SpawnedAOEActor = World->SpawnActorDeferred<AAOESkillActor>(AOEConfig.SpawnedActorClass, SpawnTransform, SpawnParams.Owner, SpawnParams.Instigator, SpawnParams.SpawnCollisionHandlingOverride);
    //AAOESkillActor* SpawnedAOEActor = World->SpawnActor<AAOESkillActor>(AOEConfig.SpawnedActorClass, SpawnTransform, SpawnParams);

    if(!SpawnedAOEActor)
    {
        UE_LOG(LogTemp, Warning, TEXT("AOE 액터 스폰에 실패했습니다. SpawnedActorClass : %s"), *GetNameSafe(AOEConfig.SpawnedActorClass));
        return nullptr;
    }

    SpawnedAOEActor->InitializeAOEActor(
        GetAvatarActorFromActorInfo(),
        AOEConfig.TargetPolicy,
        GetAbilityLevel(),
        MakeDamageSpecHandle(),
        MakeBuffDebuffSpecHandle(),
        MakeStatusSpecHandles()
    );
    SpawnedAOEActor->FinishSpawning(SpawnTransform);

    return SpawnedAOEActor;
}

FGameplayEffectSpecHandle UHeroAbility_AOEBase::MakeDamageSpecHandle()
{
    // 데미지 이펙트 클래스가 없다면 종료
    if (!AOEConfig.DamageEffectClass) return FGameplayEffectSpecHandle();

    FGameplayEffectSpecHandle SpecHandle = MakeOutgoingEffectSpec(AOEConfig.DamageEffectClass);
    if (SpecHandle.IsValid())
    {
        return SpecHandle;
    }
    return FGameplayEffectSpecHandle();
}

FGameplayEffectSpecHandle UHeroAbility_AOEBase::MakeBuffDebuffSpecHandle()
{
    // 버프/디버프 이펙트 클래스가 없다면 종료
    if (!AOEConfig.BuffDebuffClass) return FGameplayEffectSpecHandle();
    
    FGameplayEffectSpecHandle SpecHandle = MakeOutgoingEffectSpec(AOEConfig.BuffDebuffClass);
    if (SpecHandle.IsValid())
    {
        return SpecHandle;
    }
    return FGameplayEffectSpecHandle();
}

TArray<FGameplayEffectSpecHandle> UHeroAbility_AOEBase::MakeStatusSpecHandles()
{
    if (AOEConfig.StatusEffectClasses.IsEmpty()) return TArray<FGameplayEffectSpecHandle>();

    TArray<FGameplayEffectSpecHandle> SpecHandles;
    for(TSubclassOf<UGameplayEffect> StatusEffectClass : AOEConfig.StatusEffectClasses)
    {
        FGameplayEffectSpecHandle SpecHandle = MakeOutgoingEffectSpec(StatusEffectClass);
        if (SpecHandle.IsValid())
        {
            SpecHandles.Add(SpecHandle);
        }
    }
    return SpecHandles;
}