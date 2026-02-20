// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Unit/UnitAbility_SpawnProjectile.h"
#include "Items/PGProjectileBase.h"
#include "PGGameplayTags.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"

void UUnitAbility_SpawnProjectile::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    // 애니메이션 몽타주 재생
    UAnimMontage* SelectedMontage = ProjectileAttackMontages[FMath::RandRange(0, ProjectileAttackMontages.Num() - 1)];
    UAbilityTask_PlayMontageAndWait* ProjectileMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, SelectedMontage);

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

    APGProjectileBase* SpawnedProjectile = GetWorld()->SpawnActorDeferred<APGProjectileBase>(SpawnedProjectileClass, FTransform(SpawnRotation, SpawnLocation),
        GetAvatarActorFromActorInfo(), Cast<APawn>(GetAvatarActorFromActorInfo()), ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

    if (SpawnedProjectile)
    {
        float ProjectileMultiplierValue = ProjectileAttackSkillMultiplier.GetValueAtLevel(GetAbilityLevel());
        FGameplayEffectSpecHandle ProjectileDamageEffectSpecHandle = MakeOutgoingGameplayEffectSpec(ProjectileAttackDamageEffectClass, ProjectileMultiplierValue);
        SpawnedProjectile->SetProjectileDamageEffectSpecHandle(ProjectileDamageEffectSpecHandle);

        SpawnedProjectile->FinishSpawning(FTransform(SpawnRotation, SpawnLocation));
    }
}

void UUnitAbility_SpawnProjectile::OnMontageFinished()
{
    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}