// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Player/HeroAbility_BaseProjectileAttack.h"
#include "Components/Combat/HeroCombatComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "PGGameplayTags.h"
#include "Items/PGProjectileBase.h"

#include "DataAssets/Ability/AbilityConfig.h"

UHeroAbility_BaseProjectileAttack::UHeroAbility_BaseProjectileAttack()
{
    // 기본 설정
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UHeroAbility_BaseProjectileAttack::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
    Super::OnGiveAbility(ActorInfo, Spec);

    USpawnProjectileAbilityConfig* Data = Cast<USpawnProjectileAbilityConfig>(GetCurrentAbilitySpec()->SourceObject.Get());
    if (Data)
    {
        ProjectileAttackMontage = Data->AbilityMontage;
        ProjectileAttackSkillMultiplier = Data->DamageMultiplier;
        SpawnedProjectileClass = Data->SpawnedProjectileClass;
    }
}

void UHeroAbility_BaseProjectileAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    if (CachedWeaponStaticMesh == nullptr)
    {
        CachedWeaponStaticMesh = GetHeroCombatComponentFromActorInfo()->CachedWeaponMeshComponent.Get();
    }

    checkf(ProjectileAttackMontage, TEXT("ProjectileAttackMontage가 비어있습니다!"));

    // 애니메이션 몽타주 재생
    //UAnimMontage* SelectedMontage = ProjectileAttackMontages[FMath::RandRange(0, ProjectileAttackMontages.Num() - 1)];
    UAbilityTask_PlayMontageAndWait* ProjectileMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, ProjectileAttackMontage);

    // 몽타주 완료 이벤트 바인딩
    if (ProjectileMontageTask)
    {
        ProjectileMontageTask->OnCompleted.AddUniqueDynamic(this, &UHeroAbility_BaseProjectileAttack::OnMontageFinished);
        ProjectileMontageTask->OnInterrupted.AddUniqueDynamic(this, &UHeroAbility_BaseProjectileAttack::OnMontageFinished);
        ProjectileMontageTask->OnBlendOut.AddUniqueDynamic(this, &UHeroAbility_BaseProjectileAttack::OnMontageFinished);
        ProjectileMontageTask->OnCancelled.AddUniqueDynamic(this, &UHeroAbility_BaseProjectileAttack::OnMontageFinished);

        ProjectileMontageTask->ReadyForActivation();
    }

    // 게임플레이 이벤트 대기 태스크 생성
    UAbilityTask_WaitGameplayEvent* ProjectileSpawnEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, PGGameplayTags::Shared_Event_ProjectileSpawn);

    // 이벤트 수신 핸들러 바인딩
    ProjectileSpawnEventTask->EventReceived.AddUniqueDynamic(this, &UHeroAbility_BaseProjectileAttack::SpawnProjectile);
    ProjectileSpawnEventTask->ReadyForActivation();
}

void UHeroAbility_BaseProjectileAttack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UHeroAbility_BaseProjectileAttack::SpawnProjectile(FGameplayEventData InEventData)
{
    FVector SpawnLocation = CachedWeaponStaticMesh->GetSocketLocation(FName("SpawnProjectileSocket"));
    FRotator SpawnRotation = GetAvatarActorFromActorInfo()->GetActorForwardVector().Rotation();

    APGProjectileBase* SpawnedProjectile = GetWorld()->SpawnActorDeferred<APGProjectileBase>(SpawnedProjectileClass, FTransform(SpawnRotation, SpawnLocation),
        GetAvatarActorFromActorInfo(), Cast<APawn>(GetAvatarActorFromActorInfo()), ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

    // 프로젝타일에 데미지 이펙트 스펙 핸들을 설정한 후에 스폰 완료
    if (SpawnedProjectile)
    {
        float ProjectileMultiplierValue = ProjectileAttackSkillMultiplier.GetValueAtLevel(GetAbilityLevel());
        FGameplayEffectSpecHandle ProjectileDamageEffectSpecHandle = MakeOutgoingGameplayEffectSpec(ProjectileAttackDamageEffectClass, ProjectileMultiplierValue);
        SpawnedProjectile->SetProjectileDamageEffectSpecHandle(ProjectileDamageEffectSpecHandle);

        SpawnedProjectile->FinishSpawning(FTransform(SpawnRotation, SpawnLocation));
    }
}

void UHeroAbility_BaseProjectileAttack::OnMontageFinished()
{
    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}