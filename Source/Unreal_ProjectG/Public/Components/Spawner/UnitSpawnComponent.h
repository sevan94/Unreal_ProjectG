#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h" 
#include "UnitSpawnComponent.generated.h"

class AUnitCharacter;
class UDataAsset_UnitStartupData;


USTRUCT(BlueprintType)
struct FUnitSpawnInfo
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
    TSubclassOf<AUnitCharacter> UnitClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
    TSoftObjectPtr<UDataAsset_UnitStartupData> UnitData;
};


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

    UFUNCTION(BlueprintCallable, Category = "Combat")
    void SetAttackTarget(AActor* InTargetActor);
protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn Settings")
    TArray<FUnitSpawnInfo> SpawnList;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn Settings")
    float SpawnInterval = 3.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn Settings")
    bool bLoopSpawning = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn Settings")
    int32 PoolSize = 10;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
    TObjectPtr<AActor> AttackTarget;
private:
    FTimerHandle SpawnTimerHandle;
};