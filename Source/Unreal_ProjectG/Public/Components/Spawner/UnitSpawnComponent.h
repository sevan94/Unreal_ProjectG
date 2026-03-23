#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h" 
#include "Types/PGEnumTypes.h"
#include "UnitSpawnComponent.generated.h"

class AUnitCharacter;
class UDataAsset_UnitStartupData;
class UDA_StageUnitListDataAsset;

// [변경] UCLASS 설정에 ClassGroup, meta 추가 (에디터 컴포넌트 목록에 뜨게 함)
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UNREAL_PROJECTG_API UUnitSpawnComponent : public USceneComponent
{
    GENERATED_BODY()

public:
    UUnitSpawnComponent();

protected:
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
    void SpawnRandomUnit();

    UFUNCTION(BlueprintCallable, Category = "Wave")
    void StartWave();

    UFUNCTION(BlueprintCallable, Category = "Combat")
    void SetAttackTarget(AActor* InTargetActor);

    UFUNCTION(BlueprintCallable, Category = "Spawner")
    void StopWave();

protected:
    UFUNCTION()
    void OnTargetBaseDestroyed(ETeamType DestroyedTeam);
protected:
    //UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn Settings")
    //TArray<FUnitSpawnInfo> SpawnList;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn Settings")
    float SpawnInterval = 0.0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn Settings")
    bool bLoopSpawning = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn Settings")
    int32 PoolSize = 7;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
    TObjectPtr<AActor> AttackTarget = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
    UDA_StageUnitListDataAsset* StageUnitDataAsset = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
    int32 CurrentStageIndex = 0;

    FTimerHandle SpawnTimerHandle;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawn")
    bool bHasSpawnedBoss = false;
};