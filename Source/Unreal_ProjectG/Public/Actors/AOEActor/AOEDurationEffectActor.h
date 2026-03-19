// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/AOESkillActor.h"
#include "AOEDurationEffectActor.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_PROJECTG_API AAOEDurationEffectActor : public AAOESkillActor
{
	GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, meta = (DisplayName = "Collect And Apply Duration Effects"))
    void BP_CollectAndApplyDurationEffects(float InSkillMultiplier, float InBaseAmount, float InDuration);
};
