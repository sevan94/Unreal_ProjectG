#include "Actors/AOESkillActor.h"
#include "Components/SphereComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "PGFunctionLibrary.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "PGGameplayTags.h"

AAOESkillActor::AAOESkillActor()
{
	PrimaryActorTick.bCanEverTick = false;

    SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
    SetRootComponent(SceneRoot);

    CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
    CollisionSphere->SetupAttachment(SceneRoot);
    CollisionSphere->SetSphereRadius(200.f);
    CollisionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    CollisionSphere->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
    CollisionSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
    CollisionSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

void AAOESkillActor::InitializeAOEActor(AActor* InInstigator, EAOETargetPolicy InTargetPolicy, int32 InAbilityLevel, const FGameplayEffectSpecHandle& InDamageEffectSpecHandle, const FGameplayEffectSpecHandle& InBuffDebuffEffectSpecHandle, const TArray<FGameplayEffectSpecHandle>& InStatusEffectSpecHandles)
{
    InstigatorActor = InInstigator;
    TargetPolicy = InTargetPolicy;
    DamageEffectSpecHandle = InDamageEffectSpecHandle;
    BuffDebuffEffectSpecHandle = InBuffDebuffEffectSpecHandle;
    StatusEffectSpecHandles = InStatusEffectSpecHandles;
}

float AAOESkillActor::GetAOERadius() const
{
    return CollisionSphere ? CollisionSphere->GetScaledSphereRadius() : 0.f;
}

void AAOESkillActor::BP_CollectAndApplyEffects()
{
    if (!InstigatorActor.IsValid()) return;

    TArray<AActor*> IgnoredActors;
    IgnoredActors.Add(InstigatorActor.Get());

    TArray<AActor*> OverlappedActors;
    UKismetSystemLibrary::SphereOverlapActors(
        this,
        CollisionSphere->GetComponentLocation(),
        GetAOERadius(),
        TArray<TEnumAsByte<EObjectTypeQuery>>{EObjectTypeQuery::ObjectTypeQuery3}, // Pawn만 검사
        nullptr,
        IgnoredActors,
        OverlappedActors);

    // 최대 타겟 수 제한을 적용하여 유효한 타겟에게 이펙트 적용
    int32 HitCount = 0;
    for (AActor* Target : OverlappedActors)
    {
        if (MaxHitTargets > 0 && HitCount >= MaxHitTargets)
        {
            break; // 최대 타겟 수에 도달하면 루프 종료
        }
        if (IsValidTarget(Target))
        {
            ApplySpecHandlesToSingleTarget(Target);
            HitCount++;
        }
    }
}

FGameplayEffectSpecHandle AAOESkillActor::MakeDamageEffectSpecHandleInAOE(float InSkillMultiplier) const
{
    if(DamageEffectSpecHandle.IsValid())
    {
        FGameplayEffectSpecHandle NewSpecHandle = DamageEffectSpecHandle;
        if (NewSpecHandle.IsValid())
        {
            // 스킬 배율을 적용하여 데미지 계산 (예시에서는 SetByCaller로 처리)
            NewSpecHandle.Data->SetSetByCallerMagnitude(PGGameplayTags::Shared_SetByCaller_SkillMultiplier, InSkillMultiplier);
            return NewSpecHandle;
        }
    }
    return FGameplayEffectSpecHandle();
}

FGameplayEffectSpecHandle AAOESkillActor::MakeBuffDebuffEffectSpecHandleInAOE(FBuffDebuffEffectConfig& InEffectConfig) const
{
    if(BuffDebuffEffectSpecHandle.IsValid())
    {
        FGameplayEffectSpecHandle NewSpecHandle = BuffDebuffEffectSpecHandle;
        if (NewSpecHandle.IsValid())
        {
            float FinalBuffAmount = InEffectConfig.BaseBuffAmount.GetValueAtLevel(AbilityLevel) * InEffectConfig.SkillMultiplier.GetValueAtLevel(AbilityLevel);
            // 스킬 배율과 지속시간을 적용하여 버프/디버프 계산 (예시에서는 SetByCaller로 처리)
            NewSpecHandle.Data->SetSetByCallerMagnitude(InEffectConfig.BuffTag, InDuration);
            return NewSpecHandle;
        }
    }
    return FGameplayEffectSpecHandle();
}

TArray<FGameplayEffectSpecHandle> AAOESkillActor::MakeStatusEffectSpecHandlesInAOE(float InDuration) const
{
    if(StatusEffectSpecHandles.Num() > 0)
    {
        TArray<FGameplayEffectSpecHandle> NewSpecHandles;
        for (const FGameplayEffectSpecHandle& SpecHandle : StatusEffectSpecHandles)
        {
            if (SpecHandle.IsValid())
            {
                FGameplayEffectSpecHandle NewSpecHandle = SpecHandle;
                if (NewSpecHandle.IsValid())
                {
                    // 지속시간을 적용하여 상태형 버프 계산 (예시에서는 SetByCaller로 처리)
                    NewSpecHandle.Data->SetSetByCallerMagnitude(PGGameplayTags::Shared_SetByCaller_Duration, InDuration);
                    NewSpecHandles.Add(NewSpecHandle);
                }
            }
        }
        return NewSpecHandles;
    }
    return TArray<FGameplayEffectSpecHandle>();
}

bool AAOESkillActor::IsValidTarget(AActor* InTarget) const
{
    if (!InTarget || !InstigatorActor.IsValid()) return false;
    
    const bool bIsHostile = UPGFunctionLibrary::IsTargetCharacterHostile(InstigatorActor.Get(), InTarget);

    switch (TargetPolicy)
    {
    case EAOETargetPolicy::HostileOnly: return bIsHostile;
    case EAOETargetPolicy::FriendlyOnly: return !bIsHostile;
    case EAOETargetPolicy::AllExceptSelf: return true;
    default: return false;
    }
}

void AAOESkillActor::ApplySpecHandlesToSingleTarget(AActor* InTarget)
{
    UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InTarget);

    if (!TargetASC) return;

    for (const FGameplayEffectSpecHandle& SpecHandle : EffectSpecHandles)
    {
        if (SpecHandle.IsValid())
        {
            TargetASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
        }
    }
}