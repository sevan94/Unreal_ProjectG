// UnitSpawnSubsystem.h

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "UnitSpawnSubsystem.generated.h"

class AUnitCharacter;
class UDataAsset_UnitStartupData;

USTRUCT()
struct FUnitPoolList
{
    GENERATED_BODY()

    UPROPERTY()
    TArray<TObjectPtr<AUnitCharacter>> Units;
};


UCLASS()
class UNREAL_PROJECTG_API UUnitSpawnSubsystem : public UWorldSubsystem
{
    GENERATED_BODY()
public:
    // 풀 미리 채우기
    UFUNCTION(BlueprintCallable)
    void PrewarmPool(TSubclassOf<AUnitCharacter> UnitClass, int32 Count);

    // [수정] UnitClass 인자 추가 (어떤 유닛을 소환할지 알아야 함)
    UFUNCTION(BlueprintCallable)
    void SpawnUnit(TSubclassOf<AUnitCharacter> UnitClass, 
        TSoftObjectPtr<UDataAsset_UnitStartupData> UnitData,
        FVector Location, FRotator Rotation,
        AActor* InTargetActor);

    // [수정] 반환할 때는 '클래스 정보'가 아니라 '유닛 인스턴스' 자체가 필요함
    UFUNCTION(BlueprintCallable)
    void ReturnToPool(AUnitCharacter* Unit);

    UFUNCTION(BlueprintCallable)
    void GetAllActiveUnits(TArray<AUnitCharacter*>& OutUnits);

    UFUNCTION(BlueprintCallable)
    void DespawnAllActiveUnits();

    UFUNCTION(BlueprintCallable)
    void OnUnitDied(AUnitCharacter* Victim);

    UPROPERTY()
    TMap<TSubclassOf<AUnitCharacter>, FUnitPoolList> PoolStorage;

private:
    AUnitCharacter* GetUnitInstance(TSubclassOf<AUnitCharacter> UnitClass);

private:
    UPROPERTY()
    TArray<TWeakObjectPtr<AUnitCharacter>> ActiveUnits;
};