#include "Actors/AOESkillActor.h"
#include "Components/SphereComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "PGFunctionLibrary.h"

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

void AAOESkillActor::InitializeAOEActor(AActor* InInstigator, EAOETargetPolicy InTargetPolicy, FGameplayEffectSpecHandle& InEffectSpecHandle, int32 InAbilityLevel)
{
    InstigatorActor = InInstigator;
    TargetPolicy = InTargetPolicy;
    EffectSpecHandle = InEffectSpecHandle;
    AbilityLevel = InAbilityLevel;
}

float AAOESkillActor::GetAOERadius() const
{
    return CollisionSphere ? CollisionSphere->GetScaledSphereRadius() : 0.f;
}

TArray<AActor*> AAOESkillActor::CollectValidTargets()
{
    OverlappedTargets.Reset();
    if (!InstigatorActor.IsValid()) return {};

    TArray<AActor*> IgnoredActors = { InstigatorActor.Get() };
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
        if (MaxHitTargets > 0 && HitCount >= MaxHitTargets) break; // 최대 타겟 수에 도달하면 루프 종료

        if (IsValidTarget(Target))
        {
            OverlappedTargets.Add(Target);
            HitCount++;
        }
    }
    return OverlappedTargets;
}

float AAOESkillActor::GetScalableValueAtLevel(const FScalableFloat& InScalableFloat) const
{
    if (InScalableFloat.IsStatic())
    {
        return InScalableFloat.Value;
    }
    else
    {
        return InScalableFloat.GetValueAtLevel(AbilityLevel);
    }
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
