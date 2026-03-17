// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/AOESkillActor.h"
#include "AOEInstantEffectActor.generated.h"

/**
 * 딜/힐 등의 GEEC 기반의 SpecHandle 적용
 */
UCLASS()
class UNREAL_PROJECTG_API AAOEInstantEffectActor : public AAOESkillActor
{
	GENERATED_BODY()
	
public:
    UFUNCTION(BlueprintCallable, meta = (DisplayName = "Collect And Apply Effects"))
    void BP_CollectAndApplyEffects(float InSkillMultiplier);

};
