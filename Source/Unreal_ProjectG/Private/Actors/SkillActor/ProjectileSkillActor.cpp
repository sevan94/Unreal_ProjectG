// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/SkillActor/ProjectileSkillActor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "TimerManager.h"

AProjectileSkillActor::AProjectileSkillActor()
{
    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
    PrimaryActorTick.bCanEverTick = false;
}

void AProjectileSkillActor::InitFromConfig(const FHeroSpawnableConfig& InConfig, int32 InAbilityLevel)
{
    Super::InitFromConfig(InConfig, InAbilityLevel);

    if (Config.Speed > 0.f)
    {
        ProjectileMovement->InitialSpeed = Config.Speed;
        ProjectileMovement->MaxSpeed = Config.Speed;
        ProjectileMovement->bRotationFollowsVelocity = true;
        ProjectileMovement->ProjectileGravityScale = 0.f;
        ProjectileMovement->bShouldBounce = false;
    }

    if (Config.Speed > KINDA_SMALL_NUMBER && Config.MaxRange > 0.f)
    {
        SetLifeSpan(Config.MaxRange / Config.Speed);
    }

    BaseActorScale = GetActorScale3D();
    ElapsedGrowthTime = 0.f;

    SetRuntimeMultipliers(1.f, 1.f);
}

void AProjectileSkillActor::BeginPlay()
{
    Super::BeginPlay();

    if (bEnableTimeGrowth)
    {
        StartGrowthUpdateTimerIfNeeded();
    }
}

void AProjectileSkillActor::StartGrowthUpdateTimerIfNeeded()
{
    if (!GetWorld() || !bEnableTimeGrowth) return;

    UpdateGrowthByTime();

    GetWorldTimerManager().SetTimer(
        GrowthUpdateTimerHandle,
        this,
        &AProjectileSkillActor::UpdateGrowthByTime,
        GrowthUpdateInterval,
        true);
}

void AProjectileSkillActor::UpdateGrowthByTime()
{
    if (!bEnableTimeGrowth)
    {
        StopGrowthUpdateTimer();
        return;
    }

    // Owner/Instigator가 없으면 더 이상 spec rebuild 시도하지 않음
    if (!IsValid(GetOwner()) && !IsValid(GetInstigator()))
    {
        StopGrowthUpdateTimer();
        return;
    }

    ElapsedGrowthTime += GrowthUpdateInterval;

    const float Alpha = FMath::Clamp(ElapsedGrowthTime / GrowthDuration, 0.f, 1.f);
    const float NewMultiplier = FMath::Lerp(1.f, MaxGrowthMultiplier, Alpha);

    if (FMath::IsNearlyEqual(NewMultiplier, GetRuntimeScaleMultiplier()))
    {
        return;
    }

    // 단일 소스: SkillActor 런타임 배율 사용
    SetRuntimeMultipliers(NewMultiplier, NewMultiplier);
    //SetActorScale3D(BaseActorScale * GetRuntimeScaleMultiplier()); // 스케일은 나이아가라 문제로 제거
    RebuildEffectSpecsFromConfig();

    if(Alpha >= 1.f)
    {
        StopGrowthUpdateTimer();
    }
}

void AProjectileSkillActor::StopGrowthUpdateTimer()
{
    if (GetWorld())
    {
        GetWorldTimerManager().ClearTimer(GrowthUpdateTimerHandle);
    }
}

void AProjectileSkillActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    StopGrowthUpdateTimer();
    Super::EndPlay(EndPlayReason);
}