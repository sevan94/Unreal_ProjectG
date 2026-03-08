// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Player/HeroAbility_BaseProjectileAttack.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "PGGameplayTags.h"
#include "Items/PGProjectileBase.h"
#include "DataAssets/Ability/DataAsset_SkillData.h"
#include "Character/Hero/HeroCharacter.h"
#include "Components/Combat/HeroCombatComponent.h"

void UHeroAbility_BaseProjectileAttack::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
    Super::OnGiveAbility(ActorInfo, Spec);

    UDataAsset_SkillData* DataAsset = Cast<UDataAsset_SkillData>(GetCurrentAbilitySpec()->SourceObject.Get());
    if (DataAsset)
    {
        const FHeroSpawnProjectileAbilityConfig* Config = DataAsset->AbilityEntry.AbilityConfig.GetPtr<FHeroSpawnProjectileAbilityConfig>();
        if (Config)
        {
            HeroSpawnProjectileConfig = *Config;
        }
    }
}

void UHeroAbility_BaseProjectileAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    //==============================================
    // FHeroSpawnProjectileAbilityConfig의 SoftPtr 로드
    HeroSpawnProjectileConfig.DamageEffectClass.LoadSynchronous();
    HeroSpawnProjectileConfig.SpawnedProjectileClass.LoadSynchronous();
    for(TSoftObjectPtr<UAnimMontage>& Montage : HeroSpawnProjectileConfig.SpawnProjectileMontages)
    {
        Montage.LoadSynchronous();
    }
    //==============================================

    if (CachedWeaponStaticMesh == nullptr)
    {
        CachedWeaponStaticMesh = GetHeroCharacterFromActorInfo()->GetWeaponStaticMesh();
    }

    checkf(HeroSpawnProjectileConfig.SpawnProjectileMontages.Num() > 0, TEXT("SpawnProjectileMontages 배열이 비어있습니다!"));

    // 램덤하게 하나의 몽타주 선택
    UAnimMontage* SelectedMontage = HeroSpawnProjectileConfig.SpawnProjectileMontages[FMath::RandRange(0, HeroSpawnProjectileConfig.SpawnProjectileMontages.Num() - 1)].Get();

    // 애니메이션 몽타주 재생
    //UAnimMontage* SelectedMontage = ProjectileAttackMontages[FMath::RandRange(0, ProjectileAttackMontages.Num() - 1)];
    UAbilityTask_PlayMontageAndWait* ProjectileMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, SelectedMontage);
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
    // 스폰 위치는 무기 메시의 SpanwProjectileSocket 소켓 위치
    FVector SpawnLocation = CachedWeaponStaticMesh->GetSocketLocation(FName("SpawnProjectileSocket"));

    // 히어로 캐릭터의 TargetActor가 유효하다면 그쪽으로 발사, 아니라면 캐릭터가 바라보는 방향으로 발사
    FRotator SpawnRotation = GetAvatarActorFromActorInfo()->GetActorForwardVector().Rotation();
    if (GetHeroCharacterFromActorInfo()->GetHeroCombatComponent()->CurrentTarget.IsValid())
    {
        AActor* TargetActor = GetHeroCharacterFromActorInfo()->GetHeroCombatComponent()->CurrentTarget.Get();
        SpawnRotation = (TargetActor->GetActorLocation() - SpawnLocation).Rotation();
    }

    APGProjectileBase* SpawnedProjectile = GetWorld()->SpawnActorDeferred<APGProjectileBase>(HeroSpawnProjectileConfig.SpawnedProjectileClass.Get(), FTransform(SpawnRotation, SpawnLocation),
        GetAvatarActorFromActorInfo(), Cast<APawn>(GetAvatarActorFromActorInfo()), ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

    // 프로젝타일에 데미지 이펙트 스펙 핸들을 설정한 후에 스폰 완료
    if (SpawnedProjectile)
    {
        float ProjectileMultiplierValue = HeroSpawnProjectileConfig.SkillMultiplier.GetValueAtLevel(GetAbilityLevel());
        FGameplayEffectSpecHandle ProjectileDamageEffectSpecHandle = MakeOutgoingGameplayEffectSpec(HeroSpawnProjectileConfig.DamageEffectClass.Get(), ProjectileMultiplierValue);
        SpawnedProjectile->SetProjectileDamageEffectSpecHandle(ProjectileDamageEffectSpecHandle);

        SpawnedProjectile->FinishSpawning(FTransform(SpawnRotation, SpawnLocation));
    }
}

void UHeroAbility_BaseProjectileAttack::OnMontageFinished()
{
    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}