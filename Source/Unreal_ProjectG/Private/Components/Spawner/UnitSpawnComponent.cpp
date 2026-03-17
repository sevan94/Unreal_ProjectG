
#include "Components/Spawner/UnitSpawnComponent.h"
#include "Character/Unit/SubSystem/UnitSpawnSubsystem.h"
#include "Character/Unit/UnitCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "DataAssets/Spawner/DA_StageUnitListDataAsset.h"
#include "DataAssets/UI/UnitUIDataAsset.h"
#include "Pawn/BaseStructure.h"
#include "Kismet/GameplayStatics.h"
#include "Mode/Save/PGGameInstance.h"

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
        if (UPGGameInstance* GameInst = Cast<UPGGameInstance>(World->GetGameInstance()))
        {
            CurrentStageIndex = GameInst->CurrentStageIndex;
            UE_LOG(LogTemp, Log, TEXT("stage : %d"), CurrentStageIndex);

        }

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

    // 배열이 비어있는지 먼저 확인하여 크래시 방지
    if (!CurrentStageInfo || CurrentStageInfo->UnitSpawnList.IsEmpty())
    {
        return;
    }

    const TArray<FUnitSpawnDataInfo>& UnitSpawnList = CurrentStageInfo->UnitSpawnList;

    // 가중치를 무시하고 0부터 (배열 크기 - 1) 사이에서 무작위 인덱스 추출
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
    //if (!StageUnitDataAsset)
    //{
    //    return;
    //}

    //const FStageInfo* CurrentStageInfo = nullptr;
    //for (const FStageInfo& Info : StageUnitDataAsset->Stage)
    //{
    //    if (Info.StageIndex == CurrentStageIndex)
    //    {
    //        CurrentStageInfo = &Info;
    //        break;
    //    }
    //}

    //if (!CurrentStageInfo || CurrentStageInfo->UnitSpawnList.Num() == 0)
    //{
    //    return;
    //}

    //const TArray<FUnitSpawnDataInfo>& UnitSpawnList = CurrentStageInfo->UnitSpawnList;

    //int32 RandomIndex = FMath::RandRange(0, UnitSpawnList.Num() - 1);
    //TSubclassOf<AUnitCharacter> SelectedUnitClass;
    //if (UnitSpawnList[RandomIndex].UnitData)
    //{
    //    SelectedUnitClass = UnitSpawnList[RandomIndex].UnitData->UnitClass;
    //}

    //if (SelectedUnitClass)
    //{
    //    if (UWorld* World = GetWorld())
    //    {
    //        if (UUnitSpawnSubsystem* SpawnSystem = World->GetSubsystem<UUnitSpawnSubsystem>())
    //        {
    //            SpawnSystem->SpawnUnit(
    //                SelectedUnitClass,
    //                GetComponentLocation(),
    //                GetComponentRotation(),
    //                AttackTarget
    //            );
    //        }
    //    }
    //}
}

void UUnitSpawnComponent::StartWave()
{
    SpawnInterval = 1.0f;

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

    if (ABaseStructure* TargetBase = Cast<ABaseStructure>(AttackTarget))
    {
        TargetBase->OnBaseDestroyed.AddUniqueDynamic(this, &UUnitSpawnComponent::OnTargetBaseDestroyed);
    }
}

void UUnitSpawnComponent::StopWave()
{
    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().ClearTimer(SpawnTimerHandle);
    }
}

void UUnitSpawnComponent::OnTargetBaseDestroyed(ETeamType DestroyedTeam)
{
    StopWave();

    if (UWorld* World = GetWorld())
    {
        TArray<AActor*> FoundUnits;

        UGameplayStatics::GetAllActorsOfClass(World, AUnitCharacter::StaticClass(), FoundUnits);

        for (AActor* Actor : FoundUnits)
        {
            if (AUnitCharacter* Unit = Cast<AUnitCharacter>(Actor))
            {
                Unit->DeactivateUnit();
            }
        }
    }
}
