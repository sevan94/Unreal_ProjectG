// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Unit/UnitAbility_SpawnProjectile.h"
#include "Items/PGProjectileBase.h"
#include "PGGameplayTags.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "DataAssets/Ability/DataAsset_SkillData.h"

void UUnitAbility_SpawnProjectile::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
    Super::OnGiveAbility(ActorInfo, Spec);

    UDataAsset_SkillData* DataAsset = Cast<UDataAsset_SkillData>(GetCurrentAbilitySpec()->SourceObject.Get());
    if (DataAsset)
    {
        const FUnitSpawnProjectileAbilityConfig* Config = DataAsset->AbilityEntry.AbilityConfig.GetPtr<FUnitSpawnProjectileAbilityConfig>();
        if (Config)
        {
            UnitSpawnProjectileConfig = *Config;
        }
    }
}

void UUnitAbility_SpawnProjectile::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    //==============================================
// FUnitSpawnProjectileAbilityConfig의 SoftPtr 로드
    UnitSpawnProjectileConfig.DamageEffectClass.LoadSynchronous();
    UnitSpawnProjectileConfig.SpawnedProjectileClass.LoadSynchronous();
    for (TSoftObjectPtr<UAnimMontage>& Montage : UnitSpawnProjectileConfig.SpawnProjectileMontages)
    {
        Montage.LoadSynchronous();
    }
    //==============================================

    checkf(UnitSpawnProjectileConfig.SpawnProjectileMontages.Num() > 0, TEXT("ProjectileAttackMontage가 비어있습니다!"));

    // 램덤하게 하나의 몽타주 선택
    UAnimMontage* ProjectileAttackMontage = UnitSpawnProjectileConfig.SpawnProjectileMontages[FMath::RandRange(0, UnitSpawnProjectileConfig.SpawnProjectileMontages.Num() - 1)].Get();

    // 애니메이션 몽타주 재생
    UAbilityTask_PlayMontageAndWait* ProjectileMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, ProjectileAttackMontage);

    // 몽타주 완료 이벤트 바인딩
    if (ProjectileMontageTask)
    {
        ProjectileMontageTask->OnCompleted.AddUniqueDynamic(this, &UUnitAbility_SpawnProjectile::OnMontageFinished);
        ProjectileMontageTask->OnInterrupted.AddUniqueDynamic(this, &UUnitAbility_SpawnProjectile::OnMontageFinished);
        ProjectileMontageTask->OnBlendOut.AddUniqueDynamic(this, &UUnitAbility_SpawnProjectile::OnMontageFinished);
        ProjectileMontageTask->OnCancelled.AddUniqueDynamic(this, &UUnitAbility_SpawnProjectile::OnMontageFinished);

        ProjectileMontageTask->ReadyForActivation();
    }

    // 게임플레이 이벤트 대기 태스크 생성
    UAbilityTask_WaitGameplayEvent* ProjectileSpawnEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, PGGameplayTags::Shared_Event_ProjectileSpawn);

    // 이벤트 수신 핸들러 바인딩
    ProjectileSpawnEventTask->EventReceived.AddUniqueDynamic(this, &UUnitAbility_SpawnProjectile::SpawnProjectile);
    ProjectileSpawnEventTask->ReadyForActivation();
}

void UUnitAbility_SpawnProjectile::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UUnitAbility_SpawnProjectile::SpawnProjectile(FGameplayEventData InEventData)
{
    FVector SpawnLocation = GetAvatarActorFromActorInfo()->GetActorLocation();
    FRotator SpawnRotation = GetAvatarActorFromActorInfo()->GetActorForwardVector().Rotation();

    APGProjectileBase* SpawnedProjectile = GetWorld()->SpawnActorDeferred<APGProjectileBase>(UnitSpawnProjectileConfig.SpawnedProjectileClass.Get(), FTransform(SpawnRotation, SpawnLocation),
        GetAvatarActorFromActorInfo(), Cast<APawn>(GetAvatarActorFromActorInfo()), ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

    if (SpawnedProjectile)
    {
        float ProjectileMultiplierValue = UnitSpawnProjectileConfig.SkillMultiplier.GetValueAtLevel(GetAbilityLevel());
        FGameplayEffectSpecHandle ProjectileDamageEffectSpecHandle = MakeOutgoingGameplayEffectSpec(UnitSpawnProjectileConfig.DamageEffectClass.Get(), ProjectileMultiplierValue);
        SpawnedProjectile->SetProjectileDamageEffectSpecHandle(ProjectileDamageEffectSpecHandle);

        SpawnedProjectile->FinishSpawning(FTransform(SpawnRotation, SpawnLocation));
    }
}

void UUnitAbility_SpawnProjectile::OnMontageFinished()
{
    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}