// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AbilityTasks/SkillAbilityTask_MeleeTrace.h"
#include "AbilitySystem/Abilities/PGGameplayAbility.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Character/Hero/HeroCharacter.h"
#include "Components/Equipment/EquipmentsStorageComponent.h"
#include "Interfaces/EquipmentsStorageInterface.h"
#include "PGGameplayTags.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/GameplayAbilityTargetTypes.h"
#include "PGFunctionLibrary.h"

namespace MeleeTraceConstants
{
    // 트레이스 반복 간격
    // 0.016 ≈ 60fps 기준 1프레임
    constexpr float TraceInterval = 0.016f;
}

USkillAbilityTask_MeleeTrace* USkillAbilityTask_MeleeTrace::Create(UGameplayAbility* OwningAbility, const FSkillActionRow& ActionRow)
{
    USkillAbilityTask_MeleeTrace* Task = NewAbilityTask<USkillAbilityTask_MeleeTrace>(OwningAbility);
    Task->CachedActionRow = ActionRow;
    return Task;
}

void USkillAbilityTask_MeleeTrace::Activate()
{
    bHitEventFlag = false;
    const FHeroMeleeTraceConfig& Config = CachedActionRow.MeleeTraceConfig;

    UAbilityTask_WaitGameplayEvent* StartTraceEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(Ability, PGGameplayTags::Shared_Event_MeleeTraceStart);
    StartTraceEventTask->EventReceived.AddDynamic(this, &USkillAbilityTask_MeleeTrace::OnTraceStartEventReceived);
    StartTraceEventTask->ReadyForActivation();

    UAbilityTask_WaitGameplayEvent* EndTraceEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(Ability, PGGameplayTags::Shared_Event_MeleeTraceEnd);
    EndTraceEventTask->EventReceived.AddDynamic(this, &USkillAbilityTask_MeleeTrace::OnTraceEndEventReceived);
    EndTraceEventTask->ReadyForActivation();

    // 몽타주 재생
    if (Config.Montage)
    {
        UAbilityTask_PlayMontageAndWait* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(Ability, NAME_None, Config.Montage);
        MontageTask->OnCancelled.AddDynamic(this, &USkillAbilityTask_MeleeTrace::OnMontageCancelled);
        MontageTask->OnBlendOut.AddDynamic(this, &USkillAbilityTask_MeleeTrace::OnMontageCancelled);
        MontageTask->OnInterrupted.AddDynamic(this, &USkillAbilityTask_MeleeTrace::OnMontageCancelled);
        MontageTask->OnCompleted.AddDynamic(this, &USkillAbilityTask_MeleeTrace::OnMontageCancelled);
        MontageTask->ReadyForActivation();
    }
}


void USkillAbilityTask_MeleeTrace::OnMontageCancelled()
{
    OnCancelled.Broadcast({});
    EndTask();
}


void USkillAbilityTask_MeleeTrace::OnTraceStartEventReceived(FGameplayEventData Payload)
{
    bIsTraceActive = true;
    HitActors.Empty();

    // 무기 메시에서 초기 소켓 위치 가져오기
    if(IEquipmentsStorageInterface* EquipmentsStorageInterface = Cast<IEquipmentsStorageInterface>(GetAvatarActor()))
    {
        if (EquipmentsStorageInterface)
        {
            UEquipmentsStorageComponent* EquipmentsStorageComponent = EquipmentsStorageInterface->GetEquipmentsStorageComponent();
            if (EquipmentsStorageComponent)
            {
                // 소켓 이름은 "TracStart"와 "TraceEnd"이다.
                // TODO: 소켓 이름을 데이터로 관리할 수 있도록 개선 필요
                FName StartSocketName = TEXT("TraceStart");
                FName EndSocketName = TEXT("TraceEnd");
                PreviousTraceStart = EquipmentsStorageComponent->CachedWeaponMeshComponent->GetSocketLocation(StartSocketName);
                PreviousTraceEnd = EquipmentsStorageComponent->CachedWeaponMeshComponent->GetSocketLocation(EndSocketName);
            }
        }
    }

    // 트레이스 타이머 시작
    GetWorld()->GetTimerManager().SetTimer(TraceTimerHandle, this, &USkillAbilityTask_MeleeTrace::ExecuteTrace, MeleeTraceConstants::TraceInterval, true);
}

void USkillAbilityTask_MeleeTrace::OnTraceEndEventReceived(FGameplayEventData Payload)
{
    bIsTraceActive = false;
    GetWorld()->GetTimerManager().ClearTimer(TraceTimerHandle);
    HitActors.Empty();
}

void USkillAbilityTask_MeleeTrace::ExecuteTrace()
{
    if (!bIsTraceActive) return;

    FGameplayAbilityTargetDataHandle RuntimeTargetData;
    FGameplayAbilityTargetData_SingleTargetHit* HitData = new FGameplayAbilityTargetData_SingleTargetHit();

    EmitRuntimeEvent(PGGameplayTags::Event_Trigger_OnCommit, RuntimeTargetData); // 스킬 커밋 이벤트 먼저 발생시키기(예외 상황 방지 위해)

    const FHeroMeleeTraceConfig& Config = CachedActionRow.MeleeTraceConfig;
    AActor* Avatar = Ability->GetAvatarActorFromActorInfo();

    // 무기 메시에서 초기 소켓 위치 가져오기
    if (IEquipmentsStorageInterface* EquipmentsStorageInterface = Cast<IEquipmentsStorageInterface>(GetAvatarActor()))
    {
        if (EquipmentsStorageInterface)
        {
            UEquipmentsStorageComponent* EquipmentsStorageComponent = EquipmentsStorageInterface->GetEquipmentsStorageComponent();
            if (EquipmentsStorageComponent)
            {
                // 소켓 이름은 "TracStart"와 "TraceEnd"이다.
                // TODO: 소켓 이름을 데이터로 관리할 수 있도록 개선 필요
                FName StartSocketName = TEXT("TraceStart");
                FName EndSocketName = TEXT("TraceEnd");
                FVector CurrentTraceStart = EquipmentsStorageComponent->CachedWeaponMeshComponent->GetSocketLocation(StartSocketName);
                FVector CurrentTraceEnd = EquipmentsStorageComponent->CachedWeaponMeshComponent->GetSocketLocation(EndSocketName);
            
                // 트레이스 EndOffset 적용
                if (!FMath::IsNearlyZero(Config.TraceOffsetRange))
                {
                    FVector TraceDir = (CurrentTraceEnd - CurrentTraceStart).GetSafeNormal();
                    CurrentTraceEnd += TraceDir * Config.TraceOffsetRange;
                }

                EDrawDebugTrace::Type DebugTracePolicy = Config.bDrawDebugTrace ? EDrawDebugTrace::ForOneFrame : EDrawDebugTrace::None;

                TArray<FHitResult> HitResults;
                UKismetSystemLibrary::SphereTraceMultiForObjects(
                    Avatar,
                    CurrentTraceStart,   // 현재 프레임 시작점
                    CurrentTraceEnd,     // 현재 프레임 끝점
                    Config.Radius,
                    { UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn) }, // 적 캐릭터만 충돌하도록 Pawn 채널 사용
                    false,
                    TArray<AActor*>({ Avatar }),
                    DebugTracePolicy,
                    HitResults,
                    true,
                    FLinearColor::Red
                );

                UPGGameplayAbility* PGAbility = Cast<UPGGameplayAbility>(Ability);

                for (const FHitResult& HitResult : HitResults)
                {
                    AActor* HitActor = HitResult.GetActor();

                    if (!HitActor) continue;

                    // 적군만 히트
                    if (!IsValidTarget(HitActor)) continue;

                    // 중복 히트 방지
                    if (HitActors.Contains(HitActor)) continue;

                    // MaxHit 체크
                    if (HitActors.Num() >= Config.MaxHit) break;

                    // GE 적용
                    if (PGAbility && !Config.Effects.IsEmpty())
                    {
                        TArray<FGameplayEffectSpecHandle> SpecHandles = 
                            PGAbility->MakeOutgoingEffectSpecsFromEffectConfigs(Config.Effects);

                        for(const FGameplayEffectSpecHandle& SpecHandle : SpecHandles)
                        {
                            if (SpecHandle.IsValid())
                            {
                                PGAbility->NativeApplyEffectSpecHandleToTarget(HitActor, SpecHandle);
                                HitActors.Add(HitActor);

                                if (!bHitEventFlag)
                                {
                                    HitData->HitResult = HitResult;
                                    RuntimeTargetData.Add(HitData);

                                    EmitRuntimeEvent(PGGameplayTags::Event_Trigger_OnHit, RuntimeTargetData); // TODO:다른 이벤트와 혼용하지 않는지 확인
                                    bHitEventFlag = true;
                                }
                            }
                        }
                    }
                }
                PreviousTraceStart = CurrentTraceStart;
                PreviousTraceEnd = CurrentTraceEnd;
            }
        }
    }
}

bool USkillAbilityTask_MeleeTrace::IsValidTarget(AActor* Target) const
{
    return UPGFunctionLibrary::IsTargetCharacterHostile(GetAvatarActor(), Target);
}


void USkillAbilityTask_MeleeTrace::OnDestroy(bool bInOwnerFinished)
{
    // 정리
    GetWorld()->GetTimerManager().ClearTimer(TraceTimerHandle);
    HitActors.Empty();

    Super::OnDestroy(bInOwnerFinished);
}