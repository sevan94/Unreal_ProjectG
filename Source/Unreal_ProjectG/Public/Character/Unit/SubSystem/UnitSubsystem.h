// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "GameplayTagContainer.h"
#include "UnitSubsystem.generated.h"

//보스 관련 델리게이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBossSpawned, FGameplayTag, TeamTag);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBossDie, FGameplayTag, TeamTag);

USTRUCT(BlueprintType)
struct FUnitList
{
    GENERATED_BODY()

    UPROPERTY()
    TArray<TWeakObjectPtr<AActor>> Units;
};

/**
 * 
 */
UCLASS()
class UNREAL_PROJECTG_API UUnitSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
public:
    void RegisterUnit(AActor* Unit, FGameplayTag TeamTag);

    void UnregisterUnit(AActor* Unit, FGameplayTag TeamTag);

    const TArray<TWeakObjectPtr<AActor>>& GetUnitsByTeam(FGameplayTag TeamTag) const;

    FOnBossSpawned OnBossSpawnDelegate;
    FOnBossDie OnBossDeadDelegate;

private:
    UPROPERTY()
    TMap<FGameplayTag, FUnitList> TeamUnitsMap;

    TArray<TWeakObjectPtr<AActor>> EmptyArray;
};
