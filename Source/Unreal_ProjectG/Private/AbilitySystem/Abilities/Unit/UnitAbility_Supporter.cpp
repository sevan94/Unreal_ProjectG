// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/Abilities/Unit/UnitAbility_Supporter.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "PGGameplayTags.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Character/Unit/UnitCharacter.h"
#include "DataAssets/Ability/DataAsset_SkillData.h"



UUnitAbility_Supporter::UUnitAbility_Supporter()
{
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UUnitAbility_Supporter::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
    Super::OnGiveAbility(ActorInfo, Spec);

    UDataAsset_SkillData* DataAsset = Cast<UDataAsset_SkillData>(GetCurrentAbilitySpec()->SourceObject.Get());
    if (DataAsset)
    {
        const FUnitBuffAuraAbilityConfig* Config = DataAsset->AbilityEntry.AbilityConfig.GetPtr<FUnitBuffAuraAbilityConfig>();
        if (Config)
        {
            UnitBuffConfig = *Config;
        }
    }
}

void UUnitAbility_Supporter::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    //checkf(SupportMontage, TEXT("SupportMontage가 비어있습니다!"));
    UAnimMontage* MeleeAttackMontage = UnitBuffConfig.SupportMontage.Get();

    // 1. 몽타주 재생 (캐스팅 동작)
    UAbilityTask_PlayMontageAndWait* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, MeleeAttackMontage);
    if (MontageTask)
    {
        MontageTask->OnCompleted.AddUniqueDynamic(this, &UUnitAbility_Supporter::OnMontageFinished);
        MontageTask->OnInterrupted.AddUniqueDynamic(this, &UUnitAbility_Supporter::OnMontageFinished);
        MontageTask->OnCancelled.AddUniqueDynamic(this, &UUnitAbility_Supporter::OnMontageFinished);
        MontageTask->ReadyForActivation();
    }

    // 2. 애니메이션 노티파이 대기 (지팡이를 내리찍거나 손을 뻗을 때 장판 생성)
    UAbilityTask_WaitGameplayEvent* EventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, PGGameplayTags::Shared_Event_SupportExecute);
    if (EventTask)
    {
        EventTask->EventReceived.AddUniqueDynamic(this, &UUnitAbility_Supporter::HandleSupportEffect);
        EventTask->ReadyForActivation();
    }
}

void UUnitAbility_Supporter::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UUnitAbility_Supporter::HandleSupportEffect(FGameplayEventData InEventData)
{
    AActor* AvatarActor = GetAvatarActorFromActorInfo();
    if (!AvatarActor) return;

    // [중요] 지원 유닛 자신의 위치를 장판의 중심점으로 생성
    FVector SpawnLocation = AvatarActor->GetActorLocation();
    FRotator SpawnRotation = AvatarActor->GetActorForwardVector().Rotation();

    // --- [1] 시각적 장판 액터 스폰 ---
    if (UnitBuffConfig.JangpanActorClass)
    {
        AActor* SpawnedActor = GetWorld()->SpawnActorDeferred<AActor>(
            UnitBuffConfig.JangpanActorClass, FTransform(SpawnRotation, SpawnLocation),
            AvatarActor, Cast<APawn>(AvatarActor), ESpawnActorCollisionHandlingMethod::AlwaysSpawn
        );

        if (SpawnedActor)
        {
            SpawnedActor->FinishSpawning(FTransform(SpawnRotation, SpawnLocation));

            // 장판이 유닛을 따라다니게 하려면 Attach, 바닥에 고정하려면 주석 처리
            if (UnitBuffConfig.bAttachToUnit)
            {
                SpawnedActor->AttachToActor(AvatarActor, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
            }
        }
    }

    // --- [2] 자신을 중심으로 반경 내 아군 탐색 및 이펙트(힐/버프) 즉시 적용 ---
    TArray<AActor*> OverlapActors;
    TArray<AActor*> IgnoredActors;
    // 자신은 힐을 안 받게 하려면 IgnoredActors.Add(AvatarActor); 추가

    UKismetSystemLibrary::SphereOverlapActors(
        this, SpawnLocation, UnitBuffConfig.SupportRadius,
        TArray<TEnumAsByte<EObjectTypeQuery>>{ EObjectTypeQuery::ObjectTypeQuery3 }, // Pawn
        AUnitCharacter::StaticClass(), IgnoredActors, OverlapActors
    );

    float MultiplierValue = UnitBuffConfig.SupportSkillMultiplier.GetValueAtLevel(GetAbilityLevel());
    FGameplayEffectSpecHandle EffectSpecHandle = MakeOutgoingEffectSpecToTarget(UnitBuffConfig.SupportEffectClass, MultiplierValue);

    for (AActor* TargetActor : OverlapActors)
    {
        if (AUnitCharacter* TargetUnit = Cast<AUnitCharacter>(TargetActor))
        {
            //  아군 판별 로직 (예: 팀 태그 비교)
            // if (TargetUnit->GetTeamTag() == Cast<AUnitCharacter>(AvatarActor)->GetTeamTag())

            NativeApplyEffectSpecHandleToTarget(TargetActor, EffectSpecHandle);
            UE_LOG(LogTemp, Log, TEXT("지원가 장판 효과 적용됨: %s"), *TargetActor->GetName());
        }
    }
}

void UUnitAbility_Supporter::OnMontageFinished()
{
    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}