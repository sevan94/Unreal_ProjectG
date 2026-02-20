
#include "Components/Spawner/UnitSpawnComponent.h" // 경로 주의
#include "Character/Unit/SubSystem/UnitSpawnSubsystem.h"
#include "Kismet/KismetMathLibrary.h"

UUnitSpawnComponent::UUnitSpawnComponent()
{
    PrimaryComponentTick.bCanEverTick = false;

    bWantsInitializeComponent = true;
}

void UUnitSpawnComponent::BeginPlay()
{
    Super::BeginPlay();

    if (UWorld* World = GetWorld())
    {
        if (UUnitSpawnSubsystem* SpawnSystem = World->GetSubsystem<UUnitSpawnSubsystem>())
        {
            for (const FUnitSpawnInfo& UnitInfo : SpawnList)
            {
                if (UnitInfo.UnitClass)
                {
                    SpawnSystem->PrewarmPool(UnitInfo.UnitClass, PoolSize);
                }
            }
        }

        // 기존 타이머 로직
        if (SpawnList.Num() > 0)
        {
            GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &UUnitSpawnComponent::SpawnRandomUnit, SpawnInterval, bLoopSpawning);
        }
    }


}

void UUnitSpawnComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    if (GetWorld())
    {
        GetWorld()->GetTimerManager().ClearTimer(SpawnTimerHandle);
    }
    Super::EndPlay(EndPlayReason);
}

void UUnitSpawnComponent::SpawnRandomUnit()
{
    if (SpawnList.Num() == 0) return;

    int32 RandomIndex = FMath::RandRange(0, SpawnList.Num() - 1);
    const FUnitSpawnInfo& SelectedUnit = SpawnList[RandomIndex];

    if (UWorld* World = GetWorld())
    {
        if (UUnitSpawnSubsystem* SpawnSystem = World->GetSubsystem<UUnitSpawnSubsystem>())
        {
            SpawnSystem->SpawnUnit(
                SelectedUnit.UnitClass,
                SelectedUnit.UnitData,
                GetComponentLocation(),
                GetComponentRotation(),
                AttackTarget
            );
        }
    }
}

void UUnitSpawnComponent::SetAttackTarget(AActor* InTargetActor)
{
    AttackTarget = InTargetActor;
}