// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Types/AbilityConfig.h"
#include "DataAsset_HeroSkillData.generated.h"

// 스킬 액션 태그
UENUM(BlueprintType)
enum class ESkillActionType : uint8
{
    SpawnActor,
    MeleeTrace
};

// 스킬 액션 하나에 대한 단위
USTRUCT(BlueprintType)
struct FSkillActionRow
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    ESkillActionType ActionType = ESkillActionType::MeleeTrace;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "ActionType == ESkillActionType::SpawnActor", EditConditionHides))
    FHeroSpawnableConfig SpawnableConfig;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "ActionType == ESkillActionType::MeleeTrace", EditConditionHides))
    FHeroMeleeTraceConfig MeleeTraceConfig;
};

/**
 * 
 */
UCLASS()
class UNREAL_PROJECTG_API UDataAsset_HeroSkillData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FSkillActionRow> ActionSequence; // 스킬 액션 시퀀스
};
