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

    AAOESkillActor* SpawnedAOEActor = World->SpawnActor<AAOESkillActor>(AOEConfig.SpawnedActorClass, SpawnTransform, SpawnParams);

    if(!SpawnedAOEActor)
    {
        UE_LOG(LogTemp, Warning, TEXT("AOE 액터 스폰에 실패했습니다. SpawnedActorClass : %s"), *GetNameSafe(AOEConfig.SpawnedActorClass));
        return nullptr;
    }

    // TODO : 액터 초기화
    //SpawnedAOEActor->InitializeAOEActor(
    //    GetAvatarActorFromActorInfo(),
    //    BuildAllSpecHandles(),
    //    AOEConfig.TargetPolicy,
    //    AOEConfig.MaxHitTargets,
    //    AOEConfig.ImpactCueTag
    //);

    return nullptr;
    //return SpawnedAOEActor;
}

TArray<FGameplayEffectSpecHandle> UHeroAbility_AOEBase::BuildAllSpecHandles()
{
    TArray<FGameplayEffectSpecHandle> OutHandles;
    AppendDamageSpecHandle(OutHandles);
    AppendBuffDebuffSpecHandles(OutHandles);
    return OutHandles;
}

void UHeroAbility_AOEBase::AppendDamageSpecHandle(TArray<FGameplayEffectSpecHandle>& OutHandles)
{
    // 데미지 이펙트 클래스가 없다면 종료
    if (!AOEConfig.DamageConfig.DamageEffectClass) return;

    float SkillMultiplierValue = AOEConfig.DamageConfig.SkillMultiplier.GetValueAtLevel(GetAbilityLevel());

    FGameplayEffectSpecHandle SpecHandle = MakeHeroDamageEffectSpecHandle(
        AOEConfig.DamageConfig.DamageEffectClass, SkillMultiplierValue);

    if (SpecHandle.IsValid())
    {
        OutHandles.Add(SpecHandle);
    }
}

void UHeroAbility_AOEBase::AppendBuffDebuffSpecHandles(TArray<FGameplayEffectSpecHandle>& OutHandles)
{
    const FBuffDebuffConfig& BDConfig = AOEConfig.BuffDebuffConfig;
    const float Duration = BDConfig.Duration.GetValueAtLevel(GetAbilityLevel());

    for (const FNumericBuffEffectConfig& NumericBuff : BDConfig.NumericBuffs)
    {
        if (!NumericBuff.EffectClass) continue;

        const float SkillMultiplierValue =
            NumericBuff.SkillMultiplier.GetValueAtLevel(GetAbilityLevel());
        const float BaesBuffAmountValue =
            NumericBuff.BaseBuffAmount.GetValueAtLevel(GetAbilityLevel());

        FGameplayEffectSpecHandle SpecHandle = MakeDurationBuffEffectSpecHandle(
            NumericBuff.EffectClass, SkillMultiplierValue, BaesBuffAmountValue, Duration);
    }
}
