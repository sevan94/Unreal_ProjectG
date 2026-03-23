// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/SkillActor/ProjectileSkillActor.h"
#include "GameFramework/ProjectileMovementComponent.h"

AProjectileSkillActor::AProjectileSkillActor()
{
    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
}

void AProjectileSkillActor::InitFromConfig(const FHeroSpawnableConfig& InConfig, const TArray<FGameplayEffectSpecHandle>& InSpecHandles)
{
    Super::InitFromConfig(InConfig, InSpecHandles);

    if (Config.Speed > 0.f)
    {
        ProjectileMovement->InitialSpeed = Config.Speed;
        ProjectileMovement->MaxSpeed = Config.Speed;
        ProjectileMovement->bRotationFollowsVelocity = true;
        ProjectileMovement->ProjectileGravityScale = 0.f;
        ProjectileMovement->bShouldBounce = false;
    }
}