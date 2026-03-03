
#include "Components/Spawner/UnitSpawnComponent.h"
#include "Character/Unit/SubSystem/UnitSpawnSubsystem.h"
#include "Kismet/KismetMathLibrary.h"
#include "DataAssets/Spawner/DA_StageUnitListDataAsset.h"
#include "DataAssets/UI/UnitUIDataAsset.h"

UUnitSpawnComponent::UUnitSpawnComponent()
{
    PrimaryComponentTick.bCanEverTick = false;

    bWantsInitializeComponent = true;
}

void UUnitSpawnComponent::BeginPlay()
{
    Super::BeginPlay();

    if (!StageUnitDataAsset)
    {
        return;
    }

    if (UWorld* World = GetWorld())
    {
        if (UUnitSpawnSubsystem* SpawnSystem = World->GetSubsystem<UUnitSpawnSubsystem>())
        {
            TSet<TSubclassOf<AUnitCharacter>> UniqueUnitClasses;

            for (const FStageInfo& StageInfo : StageUnitDataAsset->Stage)
            {
                for (const FUnitSpawnDataInfo& UnitInfo : StageInfo.UnitSpawnList)
                {
                    if (UnitInfo.UnitData)
                    {
                        UniqueUnitClasses.Add(UnitInfo.UnitData->UnitClass);
                    }
                }
            }

            for (const TSubclassOf<AUnitCharacter>& UnitClass : UniqueUnitClasses)
            {
                SpawnSystem->PrewarmPool(UnitClass, PoolSize);
            }
        }
    }

    StartWave();
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
    if (!StageUnitDataAsset)
    {
        return;
    }

    const FStageInfo* CurrentStageInfo = nullptr;
    for (const FStageInfo& Info : StageUnitDataAsset->Stage)
    {
        if (Info.StageIndex == CurrentStageIndex)
        {
            CurrentStageInfo = &Info;
            break;
        }
    }

    if (!CurrentStageInfo || CurrentStageInfo->UnitSpawnList.Num() == 0)
    {
        return;
    }

    const TArray<FUnitSpawnDataInfo>& UnitSpawnList = CurrentStageInfo->UnitSpawnList;

    int32 RandomIndex = FMath::RandRange(0, UnitSpawnList.Num() - 1);
    TSubclassOf<AUnitCharacter> SelectedUnitClass;
    if (UnitSpawnList[RandomIndex].UnitData)
    {
        SelectedUnitClass = UnitSpawnList[RandomIndex].UnitData->UnitClass;
    }

    if (SelectedUnitClass)
    {
        if (UWorld* World = GetWorld())
        {
            if (UUnitSpawnSubsystem* SpawnSystem = World->GetSubsystem<UUnitSpawnSubsystem>())
            {
                SpawnSystem->SpawnUnit(
                    SelectedUnitClass,
                    GetComponentLocation(),
                    GetComponentRotation(),
                    AttackTarget
                );
            }
        }
    }
}

void UUnitSpawnComponent::StartWave()
{
    SpawnInterval = 2.0f;

    // 2. 타이머를 사용해 SpawnRandomUnit 함수를 지정된 간격으로 무한 반복(true) 실행
    if (GetWorld())
    {
        GetWorld()->GetTimerManager().SetTimer(
            SpawnTimerHandle,
            this,
            &UUnitSpawnComponent::SpawnRandomUnit,
            SpawnInterval,
            true // true로 설정해야 한 번만 나오지 않고 계속 반복됩니다.
        );
    }
}

void UUnitSpawnComponent::SetAttackTarget(AActor* InTargetActor)
{
    AttackTarget = InTargetActor;
}