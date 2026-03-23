// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DataAsset_SkillVisualData.generated.h"

class UNiagaraSystem;
class USoundBase;
/**
 * 
 */
UCLASS()
class UNREAL_PROJECTG_API UDataAsset_SkillVisualData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TObjectPtr<UNiagaraSystem> ProjectileVFX; // 투사체 이펙트

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TObjectPtr<UNiagaraSystem> HitVFX; // 히트 이펙트

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TObjectPtr<USoundBase> ProjectileSFX; // 투사체 사운드

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TObjectPtr<USoundBase> HitSFX; // 히트 사운드
};
