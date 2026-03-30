#include "AbilitySystem/AbilityTasks/SkillAbilityTask_SpawnActor.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Abilities/Tasks/AbilityTask_WaitTargetData.h"
#include "AbilitySystem/Abilities/PGGameplayAbility.h"
#include "AbilitySystem/PGCharacterAttributeSet.h"
#include "AbilitySystem/TargetActor/GATargetActor_AOEGroundTrace.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Actors/SkillActor/SkillActor.h"
#include "Components/Combat/HeroCombatComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "PGFunctionLibrary.h"
#include "PGGameplayTags.h"
#include "Types/PGDataTableStruct.h"

namespace SkillTaskConstants
{
    constexpr float SearchRadius = 1000.f;
}

USkillAbilityTask_SpawnActor* USkillAbilityTask_SpawnActor::Create(
    UGameplayAbility* OwningAbility,
    const FSkillActionRow& ActionRow,
    bool bIsAutoMode,
    const FGameplayAbilityTargetDataHandle& InTriggerTargetData,
    bool bInEmitRuntimeEvent)
{
    USkillAbilityTask_SpawnActor* Task = NewAbilityTask<USkillAbilityTask_SpawnActor>(OwningAbility);
    Task->CachedActionRow = ActionRow;
    Task->bAutoMode = bIsAutoMode;
    Task->TriggerTargetData = InTriggerTargetData;
    Task->bEmitRuntimeEvent = bInEmitRuntimeEvent;
    return Task;
}

void USkillAbilityTask_SpawnActor::Activate()
{
    const FHeroSpawnableConfig& Config = CachedActionRow.SpawnableConfig;
    AActor* AvatarActor = Ability->GetAvatarActorFromActorInfo();
    bWaitMontageFinish = (Config.Montage != nullptr);
    bActorSpawned = false;
    bTaskResolved = false;

    if (!AvatarActor || !Config.ActorClass)
    {
        OnCancelled.Broadcast({});
        EndTask();
        return;
    }

    switch (Config.SpawnLocationPolicy)
    {
    case ESpawnLocation::AtCaster:
        CachedSpawnLocation = AvatarActor->GetActorLocation();
        CachedSpawnRotation = AvatarActor->GetActorRotation();
        PlayMontageOrSpawn();
        break;

    case ESpawnLocation::AtTargetPoint:
        if (TryResolveSpawnFromTriggerData(CachedSpawnLocation, CachedSpawnRotation))
        {
            PlayMontageOrSpawn();
            return;
        }

        if (bAutoMode)
        {
            CachedSpawnLocation = ResolveAutoTargetLocation();
            CachedSpawnRotation = AvatarActor->GetActorRotation();
            PlayMontageOrSpawn();
        }
        else
        {
            StartWaitTargetData();
        }
        break;

    default:
        OnCancelled.Broadcast({});
        EndTask();
        break;
    }
}

void USkillAbilityTask_SpawnActor::StartWaitTargetData()
{
    const FHeroSpawnableConfig& Config = CachedActionRow.SpawnableConfig;
    AActor* AvatarActor = Ability->GetAvatarActorFromActorInfo();

    UAbilityTask_WaitTargetData* TargetTask = UAbilityTask_WaitTargetData::WaitTargetData(
        Ability,
        NAME_None,
        EGameplayTargetingConfirmation::UserConfirmed,
        AGATargetActor_AOEGroundTrace::StaticClass());

    TargetTask->ValidData.AddDynamic(this, &USkillAbilityTask_SpawnActor::OnTargetDataReady);
    TargetTask->Cancelled.AddDynamic(this, &USkillAbilityTask_SpawnActor::OnTargetDataCancelled);

    AGameplayAbilityTargetActor* SpawnedTargetActor = nullptr;
    if (TargetTask->BeginSpawningActor(Ability, AGATargetActor_AOEGroundTrace::StaticClass(), SpawnedTargetActor))
    {
        if (AGATargetActor_AOEGroundTrace* GroundTrace = Cast<AGATargetActor_AOEGroundTrace>(SpawnedTargetActor))
        {
            GroundTrace->SetOwner(AvatarActor);
            GroundTrace->SetInstigator(Cast<APawn>(AvatarActor));
            GroundTrace->PreviewRadius = Config.Radius;
            GroundTrace->AOETraceDecalMaterial = Config.IndicatorDecalMaterial;
            GroundTrace->OwnerActor = AvatarActor;
        }

        TargetTask->FinishSpawningActor(Ability, SpawnedTargetActor);
    }

    TargetTask->ReadyForActivation();
}

void USkillAbilityTask_SpawnActor::OnTargetDataReady(const FGameplayAbilityTargetDataHandle& TargetDataHandle)
{
    if (!TargetDataHandle.IsValid(0))
    {
        OnCancelled.Broadcast({});
        EndTask();
        return;
    }

    AActor* AvatarActor = Ability->GetAvatarActorFromActorInfo();
    CachedSpawnLocation = AvatarActor ? AvatarActor->GetActorLocation() : FVector::ZeroVector;
    CachedSpawnRotation = AvatarActor ? AvatarActor->GetActorRotation() : FRotator::ZeroRotator;

    if (const FGameplayAbilityTargetData* Data = TargetDataHandle.Get(0))
    {
        if (const FHitResult* Hit = Data->GetHitResult())
        {
            CachedSpawnLocation = Hit->Location;
        }
    }

    PlayMontageOrSpawn();
}

void USkillAbilityTask_SpawnActor::OnTargetDataCancelled(const FGameplayAbilityTargetDataHandle& TargetDataHandle)
{
    OnCancelled.Broadcast({});
    EndTask();
}

bool USkillAbilityTask_SpawnActor::TryResolveSpawnFromTriggerData(FVector& OutLocation, FRotator& OutRotation) const
{
    AActor* AvatarActor = Ability ? Ability->GetAvatarActorFromActorInfo() : nullptr;
    if (!AvatarActor || TriggerTargetData.Num() <= 0)
    {
        return false;
    }

    OutRotation = AvatarActor->GetActorRotation();

    for (int32 Index = 0; Index < TriggerTargetData.Num(); ++Index)
    {
        const FGameplayAbilityTargetData* Data = TriggerTargetData.Get(Index);
        if (!Data)
        {
            continue;
        }

        if (const FHitResult* Hit = Data->GetHitResult())
        {
            OutLocation = Hit->Location;
            return true;
        }

        const TArray<TWeakObjectPtr<AActor>> Actors = Data->GetActors();
        for (const TWeakObjectPtr<AActor>& WeakActor : Actors)
        {
            if (AActor* Actor = WeakActor.Get())
            {
                OutLocation = Actor->GetActorLocation();
                return true;
            }
        }
    }

    return false;
}

FVector USkillAbilityTask_SpawnActor::ResolveAutoTargetLocation() const
{
    const FHeroSpawnableConfig& Config = CachedActionRow.SpawnableConfig;
    AActor* AvatarActor = Ability->GetAvatarActorFromActorInfo();
    UHeroCombatComponent* CombatComp = UPGFunctionLibrary::NativeGetCombatComponentFromActor(AvatarActor);

    switch (Config.TargetPolicy)
    {
    case ESkillTargetPolicy::Enemy:
        if (CombatComp && CombatComp->CurrentTarget.IsValid())
        {
            return CombatComp->CurrentTarget->GetActorLocation();
        }
        break;

    case ESkillTargetPolicy::Ally:
    {
        TArray<FHitResult> HitResults;
        UKismetSystemLibrary::SphereTraceMultiForObjects(
            AvatarActor->GetWorld(),
            AvatarActor->GetActorLocation(),
            AvatarActor->GetActorLocation(),
            SkillTaskConstants::SearchRadius,
            { UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn) },
            false,
            { AvatarActor },
            EDrawDebugTrace::None,
            HitResults,
            true);

        AActor* LowestAlly = nullptr;
        float LowestHealth = FLT_MAX;

        for (const FHitResult& Hit : HitResults)
        {
            AActor* HitActor = Hit.GetActor();
            if (!HitActor)
            {
                continue;
            }

            UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(HitActor);
            if (!ASC)
            {
                continue;
            }

            const float Health = ASC->GetNumericAttribute(UPGCharacterAttributeSet::GetHealthAttribute());
            if (Health < LowestHealth)
            {
                LowestHealth = Health;
                LowestAlly = HitActor;
            }
        }

        if (LowestAlly)
        {
            return LowestAlly->GetActorLocation();
        }

        break;
    }

    case ESkillTargetPolicy::Self:
        return AvatarActor->GetActorLocation();

    default:
        break;
    }

    return AvatarActor->GetActorLocation();
}

void USkillAbilityTask_SpawnActor::PlayMontageOrSpawn()
{
    const FHeroSpawnableConfig& Config = CachedActionRow.SpawnableConfig;

    if (bWaitMontageFinish)
    {
        UAbilityTask_PlayMontageAndWait* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(Ability, NAME_None, Config.Montage);
        MontageTask->OnCompleted.AddDynamic(this, &USkillAbilityTask_SpawnActor::OnMontageFinished);
        MontageTask->OnBlendOut.AddDynamic(this, &USkillAbilityTask_SpawnActor::OnMontageFinished);
        MontageTask->OnCancelled.AddDynamic(this, &USkillAbilityTask_SpawnActor::OnMontageCancelled);
        MontageTask->OnInterrupted.AddDynamic(this, &USkillAbilityTask_SpawnActor::OnMontageCancelled);
        MontageTask->ReadyForActivation();

        UAbilityTask_WaitGameplayEvent* SpawnEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
            Ability,
            PGGameplayTags::Shared_Event_ActorSpawn);
        SpawnEventTask->EventReceived.AddDynamic(this, &USkillAbilityTask_SpawnActor::OnSpawnEventReceived);
        SpawnEventTask->ReadyForActivation();
    }
    else
    {
        SpawnActorAtLocation(CachedSpawnLocation, CachedSpawnRotation);
        bTaskResolved = true;
        OnCompleted.Broadcast({});
        EndTask();
    }
}

void USkillAbilityTask_SpawnActor::OnMontageFinished()
{
    if (bTaskResolved)
    {
        return;
    }

    if (!bActorSpawned)
    {
        SpawnActorAtLocation(CachedSpawnLocation, CachedSpawnRotation);
    }

    bTaskResolved = true;
    OnCompleted.Broadcast({});
    EndTask();
}

void USkillAbilityTask_SpawnActor::OnMontageCancelled()
{
    if (bTaskResolved)
    {
        return;
    }

    bTaskResolved = true;
    OnCancelled.Broadcast({});
    EndTask();
}

void USkillAbilityTask_SpawnActor::OnSpawnEventReceived(FGameplayEventData Payload)
{
    if (bActorSpawned)
    {
        return;
    }

    SpawnActorAtLocation(CachedSpawnLocation, CachedSpawnRotation);

    if (!bWaitMontageFinish)
    {
        if (bTaskResolved)
        {
            return;
        }
        bTaskResolved = true;
        OnCompleted.Broadcast({});
        EndTask();
    }
}

void USkillAbilityTask_SpawnActor::SpawnActorAtLocation(const FVector& Location, const FRotator& Rotation)
{
    const FHeroSpawnableConfig& Config = CachedActionRow.SpawnableConfig;
    AActor* AvatarActor = Ability->GetAvatarActorFromActorInfo();

    FVector FinalLocation = Location;
    FRotator FinalRotation = Rotation;

    // At Caster 정책이라면 이 시점에서 스폰될 위치를 결정
    if (Config.SpawnLocationPolicy == ESpawnLocation::AtCaster && AvatarActor)
    {
        FinalLocation = AvatarActor->GetActorLocation();
        FinalRotation = AvatarActor->GetActorRotation();
    }

    FVector SpawnOffset = FVector::ZeroVector;
    if (const FSpawnOffsetRow* Row = Config.SpawnOffsetRow.GetRow<FSpawnOffsetRow>(TEXT("SpawnOffset")))
    {
        SpawnOffset = Row->SpawnOffset;
    }

    // X/Y 오프셋은 캐릭터의 로컬 좌표계를 따름, Z 오프셋은 항상 월드 좌표계를 따름
    FVector RotatedOffset = SpawnOffset;
    if (AvatarActor)
    {
        const FRotator YawOnlyRotation(0.0f, AvatarActor->GetActorRotation().Yaw, 0.0f);
        const FVector XYOffset = FVector(SpawnOffset.X, SpawnOffset.Y, 0.0f);
        const FVector RotatedXYOffset = YawOnlyRotation.RotateVector(XYOffset);
        RotatedOffset = FVector(RotatedXYOffset.X, RotatedXYOffset.Y, SpawnOffset.Z);
    }

    const FTransform SpawnTransform(FinalRotation, FinalLocation + RotatedOffset);

    ASkillActor* Spawned = GetWorld()->SpawnActorDeferred<ASkillActor>(
        Config.ActorClass,
        SpawnTransform,
        AvatarActor,
        Cast<APawn>(AvatarActor),
        ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

    if (!Spawned)
    {
        OnCancelled.Broadcast({});
        EndTask();
        return;
    }
    Spawned->SetOwner(AvatarActor);
    Spawned->SetInstigator(Cast<APawn>(AvatarActor));

    Spawned->InitFromConfig(Config, Ability->GetAbilityLevel());
    Spawned->FinishSpawning(SpawnTransform);
    bActorSpawned = true;

    EmitRuntimeEvent(PGGameplayTags::Event_Trigger_OnCommit, FGameplayAbilityTargetDataHandle());
}

void USkillAbilityTask_SpawnActor::OnDestroy(bool bInOwnerFinished)
{
    Super::OnDestroy(bInOwnerFinished);
}
