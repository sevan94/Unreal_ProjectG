#include "AbilitySystem/AbilityTasks/SkillAbilityTask_SpawnActor.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Abilities/Tasks/AbilityTask_WaitTargetData.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "AbilitySystem/TargetActor/GATargetActor_AOEGroundTrace.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Actors/SkillActor/SkillActor.h"
#include "PGGameplayTags.h"
#include "Types/PGDataTableStruct.h"

#include "PGFunctionLibrary.h"
#include "Components/Combat/HeroCombatComponent.h"
#include "AbilitySystem/Abilities/PGGameplayAbility.h"
#include "AbilitySystem/PGCharacterAttributeSet.h"

// TODO : 사거리에 대하여 정확한 기획이 없어서 일단 상수로 정의해두었지만, 추후 기획에 따라 수정 필요
namespace SkillTaskConstatns
{
    constexpr float SearchRadius = 1000.f;
}


USkillAbilityTask_SpawnActor* USkillAbilityTask_SpawnActor::Create(UGameplayAbility* OwningAbility, const FSkillActionRow& ActionRow, bool bIsAutoMode)
{
    // Task 생성 및 초기화
    USkillAbilityTask_SpawnActor* Task = NewAbilityTask<USkillAbilityTask_SpawnActor>(OwningAbility);
    Task->CachedActionRow = ActionRow;
    Task->bAutoMode = bIsAutoMode;

    return Task;
}

void USkillAbilityTask_SpawnActor::Activate()
{
    const FHeroSpawnableConfig& Config = CachedActionRow.SpawnableConfig;
    AActor* AvatarActor = Ability->GetAvatarActorFromActorInfo();
    
    // 액터 클래스가 없으면 Task 취소
    if (!Config.ActorClass)
    {
        OnCancelled.Broadcast({});
        EndTask();
        return;
    }

    // 소환 위치 정책에 따라 타겟 데이터 대기 시작
    switch (Config.SpawnLocationPolicy)
    {
    case ESpawnLocation::AtCaster:
            // 캐스터 위치에서 즉시 소환
            CachedSpawnLocation = AvatarActor->GetActorLocation();
            CachedSpawnRotation = AvatarActor->GetActorRotation();
            PlayMontageOrSpawn();
            break;

    case ESpawnLocation::AtTargetPoint:
            // 타겟 포인트에 소환 -> 자동 모드면 TargetPolicy에 따라 위치 계산, 수동 모드면 WaitTargetData로 위치 확보
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

// =================================================
// 1단계 : 위치 확보
// =================================================
void USkillAbilityTask_SpawnActor::StartWaitTargetData()
{
    const FHeroSpawnableConfig& Config = CachedActionRow.SpawnableConfig;
    AActor* AvatarActor = Ability->GetAvatarActorFromActorInfo();

    UAbilityTask_WaitTargetData* TargetTask = UAbilityTask_WaitTargetData::WaitTargetData(
        Ability, NAME_None, EGameplayTargetingConfirmation::UserConfirmed, AGATargetActor_AOEGroundTrace::StaticClass());

    TargetTask->ValidData.AddDynamic(this, &USkillAbilityTask_SpawnActor::OnTargetDataReady);
    TargetTask->Cancelled.AddDynamic(this, &USkillAbilityTask_SpawnActor::OnTargetDataCancelled);
    
    AGameplayAbilityTargetActor* SpawnedTargetActor = nullptr;
    if (TargetTask->BeginSpawningActor(
        Ability, AGATargetActor_AOEGroundTrace::StaticClass(), SpawnedTargetActor))
    {
        if(AGATargetActor_AOEGroundTrace* GroundTrace = Cast<AGATargetActor_AOEGroundTrace>(SpawnedTargetActor))
        {
            // Spawn된 TargetActor 초기화
            GroundTrace->SetOwner(AvatarActor);
            GroundTrace->SetInstigator(Cast<APawn>(AvatarActor));
            GroundTrace->PreviewRadius = Config.Radius;
            GroundTrace->AOETraceDecalMaterial = Config.IndicatorDecalMaterial;
            GroundTrace->OwnerActor = AvatarActor;
        }
        TargetTask->FinishSpawningActor(Ability, SpawnedTargetActor);
    }

    // TargetData 대기 시작
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

    // TargetData에서 위치 정보 추출
    AActor* AvatarActor = Ability->GetAvatarActorFromActorInfo();
    CachedSpawnLocation = AvatarActor->GetActorLocation();
    CachedSpawnRotation = AvatarActor->GetActorRotation();

    if (const FGameplayAbilityTargetData* Data = TargetDataHandle.Get(0))
    {
        if (const FHitResult* Hit = Data->GetHitResult())
            CachedSpawnLocation = Hit->Location;
    }

    // 위치 확보 완료 -> 몽타주 재생 또는 즉시 소환
    PlayMontageOrSpawn();
}

void USkillAbilityTask_SpawnActor::OnTargetDataCancelled(const FGameplayAbilityTargetDataHandle& TargetDataHandle)
{
    OnCancelled.Broadcast({});
    EndTask();
}

FVector USkillAbilityTask_SpawnActor::ResolveAutoTargetLocation() const
{
    const FHeroSpawnableConfig& Config = CachedActionRow.SpawnableConfig;
    AActor* AvatarActor = Ability->GetAvatarActorFromActorInfo();
    UHeroCombatComponent* CombatComp = UPGFunctionLibrary::NativeGetCombatComponentFromActor(AvatarActor);

    // 스킬 타겟팅 정책에 따라 자동으로 위치 결정
    switch (Config.TargetPolicy)
    {
    case ESkillTargetPolicy::Enemy:
    {
        // 타겟팅 정책이 적이고 히어로의 현재 타겟이 유효하다면 그 위치 반환
        if (CombatComp && CombatComp->CurrentTarget.IsValid())
        {
            return CombatComp->CurrentTarget->GetActorLocation();
            break;
        }
    }
    case ESkillTargetPolicy::Ally:
    {
        // 타겟팅 정책이 아군이라면 가장 체력이 낮은 아군을 찾아서 그 위치 반환
        TArray<FHitResult> HitResults;
        UKismetSystemLibrary::SphereTraceMultiForObjects(
            AvatarActor->GetWorld(),
            AvatarActor->GetActorLocation(),
            AvatarActor->GetActorLocation(),
            SkillTaskConstatns::SearchRadius,
            { UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn) },
            false,
            { AvatarActor },
            EDrawDebugTrace::None,
            HitResults,
            true);

        // 히트 결과에서 가장 체력이 낮은 아군 찾기
        AActor* LowestAlly = nullptr;
        float LowestHealth = FLT_MAX;

        for (const FHitResult& Hit : HitResults)
        {
            AActor* HitActor = Hit.GetActor();
            if (!HitActor) continue;

            UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(HitActor);
            if (!ASC) continue;

            float Health = ASC->GetNumericAttribute(UPGCharacterAttributeSet::GetHealthAttribute());
            if (Health < LowestHealth)
            {
                LowestHealth = Health;
                LowestAlly = HitActor;
            }
        }

        // 가장 체력이 낮은 아군이 있다면 그 위치 반환
        if (LowestAlly) return LowestAlly->GetActorLocation();
        break;
    }
    case ESkillTargetPolicy::Self:
    {
        // 타겟팅 정책이 자기 자신이라면 자신의 위치 반환
        return AvatarActor->GetActorLocation();
        break;
    }
    }

    return AvatarActor->GetActorLocation();
}

// =================================================
// 2단계 : 몽타주 재생 또는 즉시 소환
// =================================================
void USkillAbilityTask_SpawnActor::PlayMontageOrSpawn()
{
    const FHeroSpawnableConfig& Config = CachedActionRow.SpawnableConfig;

    // Montage가 있다면 재생
    if (Config.Montage)
    {
        UAbilityTask_PlayMontageAndWait* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(Ability, NAME_None, Config.Montage);
        MontageTask->OnCancelled.AddDynamic(this, &USkillAbilityTask_SpawnActor::OnMontageCancelled);
        MontageTask->OnInterrupted.AddDynamic(this, &USkillAbilityTask_SpawnActor::OnMontageCancelled);
        MontageTask->ReadyForActivation();

        // AnimNotify로부터 Spawn 이벤트 수신 대기
        UAbilityTask_WaitGameplayEvent* SpawnEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
            Ability, PGGameplayTags::Shared_Event_ActorSpawn);
        SpawnEventTask->EventReceived.AddDynamic(this, &USkillAbilityTask_SpawnActor::OnSpawnEventReceived);
        SpawnEventTask->ReadyForActivation();
    }
    else
    {
        // 몽타주 없음 -> 즉시 소환 위치 결정
        SpawnActorAtLocation(CachedSpawnLocation, CachedSpawnRotation);
    }
}

void USkillAbilityTask_SpawnActor::OnMontageCancelled()
{
    OnCancelled.Broadcast({});
    EndTask();
}

void USkillAbilityTask_SpawnActor::OnSpawnEventReceived(FGameplayEventData Payload)
{
    // AnimNotify에서 이벤트 수신 -> 소환 위치 결정
    SpawnActorAtLocation(CachedSpawnLocation, CachedSpawnRotation);
}

// =================================================
// 3단계 : 액터 소환 및 파괴 대기
// =================================================
void USkillAbilityTask_SpawnActor::SpawnActorAtLocation(const FVector& Location, const FRotator& Rotation)
{
    const FHeroSpawnableConfig& Config = CachedActionRow.SpawnableConfig;
    AActor* AvatarActor = Ability->GetAvatarActorFromActorInfo();

    // Config에서 SpawnOffset이 있다면 위치에 반영
    FVector SpawnOffset = FVector::ZeroVector;
    if (const FSpawnOffsetRow* Row = Config.SpawnOffsetRow.GetRow<FSpawnOffsetRow>(TEXT("SpawnOffset")))
    {
        SpawnOffset = Row->SpawnOffset;
    }

    const FTransform SpawnTransform(Rotation, Location + SpawnOffset);

    // Deferred 스폰으로 액터의 초기화 보장
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

    // 액터 초기화
    UPGGameplayAbility* PGAbility = Cast<UPGGameplayAbility>(Ability);
    TArray<FGameplayEffectSpecHandle> SpecHandles;
    if (PGAbility && !Config.Effects.IsEmpty())
    {
        SpecHandles = PGAbility->MakeOutgoingEffectSpecsFromEffectConfigs(Config.Effects);
    }

    // Deferred 스폰 완료
    Spawned->InitFromConfig(Config, SpecHandles, Ability->GetAbilityLevel());
    Spawned->FinishSpawning(SpawnTransform);

    OnCompleted.Broadcast({});
    EndTask();
}
//=================================================
// 정리
//=================================================
void USkillAbilityTask_SpawnActor::OnDestroy(bool bInOwnerFinished)
{
    Super::OnDestroy(bInOwnerFinished);
}