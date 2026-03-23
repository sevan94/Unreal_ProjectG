// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/SkillActor/SkillActor.h"
#include "ProjectileSkillActor.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_PROJECTG_API AProjectileSkillActor : public ASkillActor
{
	GENERATED_BODY()
	
public:
    AProjectileSkillActor();

protected:
    virtual void InitFromConfig(const FHeroSpawnableConfig& InConfig, const TArray<FGameplayEffectSpecHandle>& InSpecHandles) override;


protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SkillActor|Components")
    TObjectPtr<UProjectileMovementComponent> ProjectileMovement;
};
