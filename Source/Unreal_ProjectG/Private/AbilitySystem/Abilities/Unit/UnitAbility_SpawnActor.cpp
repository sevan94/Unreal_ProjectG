// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/Abilities/Unit/UnitAbility_SpawnActor.h"
#include "PGGameplayTags.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "DataAssets/Ability/DataAsset_SkillData.h"

void UUnitAbility_SpawnActor::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
    Super::OnGiveAbility(ActorInfo, Spec);

    UDataAsset_SkillData* DataAsset = Cast<UDataAsset_SkillData>(GetCurrentAbilitySpec()->SourceObject.Get());
    if (DataAsset)
    {
        const FUnitSpawnActorAbilityConfig* Config = DataAsset->AbilityEntry.AbilityConfig.GetPtr<FUnitSpawnActorAbilityConfig>();
        if (Config)
        {
            UnitSpawnActorConfig = *Config;
        }
    }
}

void UUnitAbility_SpawnActor::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    //==============================================
    // FUnitSpawnActorConfig의 SoftPtr 로드
    UnitSpawnActorConfig.DamageEffectClass.LoadSynchronous();
    UnitSpawnActorConfig.SpawnedActorClass.LoadSynchronous();
    for (TSoftObjectPtr<UAnimMontage>& Montage : UnitSpawnActorConfig.SpawnActorMontages)
    {
        Montage.LoadSynchronous();
    }
    //==============================================

    checkf(UnitSpawnActorConfig.SpawnActorMontages.Num() > 0, TEXT("SpawnMontage가 비어있습니다!"));

    // 램덤하게 하나의 몽타주 선택
    UAnimMontage* SpawnMontage = UnitSpawnActorConfig.SpawnActorMontages[FMath::RandRange(0, UnitSpawnActorConfig.SpawnActorMontages.Num() - 1)].Get();

    // 애니메이션 몽타주 재생 태스크
    UAbilityTask_PlayMontageAndWait* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, SpawnMontage);

    if (MontageTask)
    {
        MontageTask->OnCompleted.AddUniqueDynamic(this, &UUnitAbility_SpawnActor::OnMontageFinished);
        MontageTask->OnInterrupted.AddUniqueDynamic(this, &UUnitAbility_SpawnActor::OnMontageFinished);
        MontageTask->OnBlendOut.AddUniqueDynamic(this, &UUnitAbility_SpawnActor::OnMontageFinished);
        MontageTask->OnCancelled.AddUniqueDynamic(this, &UUnitAbility_SpawnActor::OnMontageFinished);

        MontageTask->ReadyForActivation();
    }

    UAbilityTask_WaitGameplayEvent* SpawnEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, PGGameplayTags::Shared_Event_ActorSpawn, nullptr, false, false);
    if (SpawnEventTask)
    {
        SpawnEventTask->EventReceived.AddUniqueDynamic(this, &UUnitAbility_SpawnActor::SpawnActorEvent);
        SpawnEventTask->ReadyForActivation();
    }

    //UAbilityTask_WaitGameplayEvent* SpawnEventTask_Special = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, PGGameplayTags::Unit_Ability_SpecialAttack, nullptr, false, false);
    //if (SpawnEventTask_Special)
    //{
    //    SpawnEventTask_Special->EventReceived.AddUniqueDynamic(this, &UUnitAbility_SpawnActor::SpawnActorEvent);
    //    SpawnEventTask_Special->ReadyForActivation();
    //}
}

void UUnitAbility_SpawnActor::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UUnitAbility_SpawnActor::SpawnActorEvent(FGameplayEventData InEventData)
{
    if (!UnitSpawnActorConfig.SpawnedActorClass.IsValid())
    {
        return;
    }

    AActor* AvatarActor = GetAvatarActorFromActorInfo();
    if (!AvatarActor)
    {
        return;
    }

    FVector BaseLocation = AvatarActor->GetActorLocation();
    FRotator SpawnRotation = AvatarActor->GetActorForwardVector().Rotation();

    UE_LOG(LogTemp, Log, TEXT("액터스폰 시작 %d마리"), UnitSpawnActorConfig.SpawnCount);
    int32 CountToSpawn = FMath::Max(1, UnitSpawnActorConfig.SpawnCount);

    for (int32 i = 0; i < CountToSpawn; i++)
    {
        FVector SpawnLocation = BaseLocation;
        if (CountToSpawn > 1)
        {
            float OffsetY = (i - (CountToSpawn - 1) / 2.0f) * 100.0f;
            SpawnLocation += AvatarActor->GetActorRightVector() * OffsetY;
        }

        AActor* SpawnedActor = GetWorld()->SpawnActorDeferred<AActor>(
            UnitSpawnActorConfig.SpawnedActorClass.Get(),
            FTransform(SpawnRotation, SpawnLocation),
            AvatarActor,
            Cast<APawn>(AvatarActor),
            ESpawnActorCollisionHandlingMethod::AlwaysSpawn
        );

        if (SpawnedActor)
        {
            if (UnitSpawnActorConfig.DamageEffectClass.IsValid())
            {
                float MultiplierValue = UnitSpawnActorConfig.SkillMultiplier.GetValueAtLevel(GetAbilityLevel());
                FGameplayEffectSpecHandle EffectSpecHandle = MakeOutgoingGameplayEffectSpec(UnitSpawnActorConfig.DamageEffectClass.Get(), MultiplierValue);
            }

            SpawnedActor->FinishSpawning(FTransform(SpawnRotation, SpawnLocation));

            //SpawnedActor->AttachToActor(AvatarActor, FAttachmentTransformRules::KeepWorldTransform);
        }
    }
}

void UUnitAbility_SpawnActor::OnMontageFinished()
{
    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}