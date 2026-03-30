// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
//#include "NiagaraSystem.h"  
//#include "Sound/SoundBase.h"     
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
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "비행 이펙트"))
    TSoftObjectPtr<UNiagaraSystem> ProjectileVFX; // 투사체 이펙트

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "비행 사운드"))
    TSoftObjectPtr<USoundBase> ProjectileSFX; // 투사체 사운드

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "히트 이펙트"))
    TSoftObjectPtr<UNiagaraSystem> HitVFX; // 히트 이펙트

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "히트 사운드"))
    TSoftObjectPtr<USoundBase> HitSFX; // 히트 사운드

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "임팩트 이펙트"))
    TSoftObjectPtr<UNiagaraSystem> ImpactVFX; // 임팩트 이펙트

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "임팩트 사운드"))
    TSoftObjectPtr<USoundBase> ImpactSFX; // 임팩트 사운드

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "지속 이펙트"))
    TSoftObjectPtr<UNiagaraSystem> PersistentVFX; // 지속 이펙트

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "지속 사운드"))
    TSoftObjectPtr<USoundBase> PersistentSFX; // 지속 사운드
};
