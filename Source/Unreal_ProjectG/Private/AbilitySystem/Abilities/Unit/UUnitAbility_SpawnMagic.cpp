#include "AbilitySystem/Abilities/Unit/UUnitAbility_SpawnMagic.h"
#include "Actors/PGMageMagicBase.h" 
#include "PGGameplayTags.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "DataAssets/Ability/DataAsset_SkillData.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"

void UUnitAbility_SpawnMagic::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
    Super::OnGiveAbility(ActorInfo, Spec);

    UDataAsset_SkillData* DataAsset = Cast<UDataAsset_SkillData>(GetCurrentAbilitySpec()->SourceObject.Get());
    if (DataAsset)
    {
        // 구조체 타입을 FUnitSpawnMagicAbilityConfig로 받음
        const FUnitSpawnMagicAbilityConfig* Config = DataAsset->AbilityEntry.AbilityConfig.GetPtr<FUnitSpawnMagicAbilityConfig>();
        if (Config)
        {
            UnitSpawnMagicConfig = *Config;
        } 
    }
}

void UUnitAbility_SpawnMagic::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    // SoftPtr 동기 로드
    UnitSpawnMagicConfig.SpawnedMagicClass.LoadSynchronous();
    for (TSoftObjectPtr<UAnimMontage>& Montage : UnitSpawnMagicConfig.SpawnMagicMontages)
    {
        Montage.LoadSynchronous();
    }

    checkf(UnitSpawnMagicConfig.SpawnMagicMontages.Num() > 0, TEXT("SpawnMagicMontages가 비어있습니다!"));

    // 랜덤 몽타주 선택 및 재생
    UAnimMontage* MagicAttackMontage = UnitSpawnMagicConfig.SpawnMagicMontages[FMath::RandRange(0, UnitSpawnMagicConfig.SpawnMagicMontages.Num() - 1)].Get();
    UAbilityTask_PlayMontageAndWait* MagicMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, MagicAttackMontage);

    if (MagicMontageTask)
    {
        MagicMontageTask->OnCompleted.AddUniqueDynamic(this, &UUnitAbility_SpawnMagic::OnMontageFinished);
        MagicMontageTask->OnInterrupted.AddUniqueDynamic(this, &UUnitAbility_SpawnMagic::OnMontageFinished);
        MagicMontageTask->OnBlendOut.AddUniqueDynamic(this, &UUnitAbility_SpawnMagic::OnMontageFinished);
        MagicMontageTask->OnCancelled.AddUniqueDynamic(this, &UUnitAbility_SpawnMagic::OnMontageFinished);
        MagicMontageTask->ReadyForActivation();
    }

    // 마법 시전 타이밍 이벤트 대기
    UAbilityTask_WaitGameplayEvent* MagicSpawnEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, PGGameplayTags::Shared_Event_ActorSpawn); 

    MagicSpawnEventTask->EventReceived.AddUniqueDynamic(this, &UUnitAbility_SpawnMagic::SpawnMagic);
    MagicSpawnEventTask->ReadyForActivation();
}

void UUnitAbility_SpawnMagic::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UUnitAbility_SpawnMagic::SpawnMagic(FGameplayEventData InEventData)
{
    FVector SpawnLocation = GetAvatarActorFromActorInfo()->GetActorLocation();
    FRotator SpawnRotation = GetAvatarActorFromActorInfo()->GetActorRotation();

    if (ACharacter* AvatarChar = Cast<ACharacter>(GetAvatarActorFromActorInfo()))
    {
        SpawnLocation = AvatarChar->GetMesh()->GetSocketLocation(FName("MagicSpawnSocket"));
        SpawnRotation = AvatarChar->GetMesh()->GetSocketRotation(FName("MagicSpawnSocket"));
    }
    else if (APawn* AvatarPawn = Cast<APawn>(GetAvatarActorFromActorInfo()))
    {
        SpawnRotation = AvatarPawn->GetBaseAimRotation();
    }

    APGMageMagicBase* SpawnedMagic = GetWorld()->SpawnActorDeferred<APGMageMagicBase>(
        UnitSpawnMagicConfig.SpawnedMagicClass.Get(),
        FTransform(SpawnRotation, SpawnLocation),
        GetAvatarActorFromActorInfo(),
        Cast<APawn>(GetAvatarActorFromActorInfo()),
        ESpawnActorCollisionHandlingMethod::AlwaysSpawn
    );

    if (SpawnedMagic)
    {
        float MagicMultiplierValue = UnitSpawnMagicConfig.SkillMultiplier.GetValueAtLevel(GetAbilityLevel());

        TArray<FGameplayEffectSpecHandle> MagicDamageEffectSpecHandles;

        for (const TSubclassOf<UGameplayEffect>& EffectClass : UnitSpawnMagicConfig.DamageEffectClass)
        {
            if (EffectClass) 
            {
                FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(EffectClass, MagicMultiplierValue);
                MagicDamageEffectSpecHandles.Add(SpecHandle);
            }
        }

        SpawnedMagic->SetMagicDamageEffectSpecHandle(MagicDamageEffectSpecHandles);

        SpawnedMagic->FinishSpawning(FTransform(SpawnRotation, SpawnLocation));
    }
}

void UUnitAbility_SpawnMagic::OnMontageFinished()
{
    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}