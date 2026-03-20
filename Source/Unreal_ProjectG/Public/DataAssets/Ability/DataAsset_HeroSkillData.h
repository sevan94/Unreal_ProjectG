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
    ESkillActionType ActionType;

    // 비어있다면 이전 액션 완료 후 즉시 실행
    // 이 액션이 실행될 때 필요한 태그
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FGameplayTag WaitEventTag;

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
