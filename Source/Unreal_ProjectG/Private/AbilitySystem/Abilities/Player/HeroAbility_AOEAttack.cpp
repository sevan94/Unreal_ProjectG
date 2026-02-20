// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Player/HeroAbility_AOEAttack.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "PGGameplayTags.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Character/Unit/UnitCharacter.h"
#include "PGFunctionLibrary.h"
#include "GameplayCueFunctionLibrary.h"

UHeroAbility_AOEAttack::UHeroAbility_AOEAttack()
{
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UHeroAbility_AOEAttack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
    HitActors.Empty();

    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UHeroAbility_AOEAttack::OnHitLocationReady(FVector InHitLocation)
{
    CachedHitLocation = InHitLocation;

    // 애니메이션 몽타주 재생
    UAbilityTask_PlayMontageAndWait* AOEMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, AOEAttackMontage);

    // 몽타주 완료 이벤트 바인딩
    if (AOEMontageTask)
    {
        UE_LOG(LogTemp, Warning, TEXT("AOE Attack Montage Play!"));

        AOEMontageTask->OnCompleted.AddUniqueDynamic(this, &UHeroAbility_AOEAttack::OnAOEMontageFinished);
        AOEMontageTask->OnInterrupted.AddUniqueDynamic(this, &UHeroAbility_AOEAttack::OnAOEMontageFinished);
        AOEMontageTask->OnBlendOut.AddUniqueDynamic(this, &UHeroAbility_AOEAttack::OnAOEMontageFinished);
        AOEMontageTask->OnCancelled.AddUniqueDynamic(this, &UHeroAbility_AOEAttack::OnAOEMontageFinished);

        AOEMontageTask->ReadyForActivation();
    }

    // 게임 플레이 이벤트 대기 태스크 생성
    UAbilityTask_WaitGameplayEvent* AOEAttackEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, PGGameplayTags::Shared_Event_AOEExecute);

    // 이벤트 수신 핸들러 바인딩
    AOEAttackEventTask->EventReceived.AddUniqueDynamic(this, &UHeroAbility_AOEAttack::OnApplyAOEDamage);
    AOEAttackEventTask->ReadyForActivation();
}

void UHeroAbility_AOEAttack::OnApplyAOEDamage(FGameplayEventData EventData)
{
    FGameplayCueParameters GameplayCueParameters;
    GameplayCueParameters.Location = CachedHitLocation;
    UGameplayCueFunctionLibrary::ExecuteGameplayCueOnActor(GetAvatarActorFromActorInfo(), AOEImpactCueTag, GameplayCueParameters);


    // 무시할 액터 설정
    TArray<AActor*> IgnoredActors;
    IgnoredActors.Add(GetAvatarActorFromActorInfo());
    // 히트 위치 주변의 액터 오버랩 검사
    UKismetSystemLibrary::SphereOverlapActors(
        this,
        CachedHitLocation,
        AOEAttackRadius,
        TArray<TEnumAsByte<EObjectTypeQuery>>{ EObjectTypeQuery::ObjectTypeQuery3 }, // Pawn 객체 타입
        AUnitCharacter::StaticClass(),
        IgnoredActors,
        HitActors
    );

    float SkillMultiplierValue = AOEAttackSkillMultiplier.GetValueAtLevel(GetAbilityLevel());
    FGameplayEffectSpecHandle EffectSpecHandle = MakeHeroDamageEffectSpecHandle(AOEAttackDamageEffectClass, SkillMultiplierValue);

    for (AActor* HitActor : HitActors)
    {
        if (UPGFunctionLibrary::IsTargetCharacterIsHostile(GetAvatarActorFromActorInfo(), HitActor))
        {
            NativeApplyEffectSpecHandleToTarget(HitActor, EffectSpecHandle);
        }
    }
}

void UHeroAbility_AOEAttack::OnAOEMontageFinished()
{
    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
