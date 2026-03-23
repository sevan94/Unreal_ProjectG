
#include "Components/Spawner/UnitSpawnComponent.h"
#include "Character/Unit/SubSystem/UnitSpawnSubsystem.h"
#include "Character/Unit/UnitCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "DataAssets/Spawner/DA_StageUnitListDataAsset.h"
#include "DataAssets/UI/EnemyUIDataAsset.h"
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

                for (const FUnitSpawnDataInfo& UnitInfo : StageUnitDataAsset->UnitSpawnList)
                {
                    if (UnitInfo.UnitData)
                    {
                        UniqueUnitClasses.Add(UnitInfo.UnitData->EnemyClass);
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
    const TArray<FUnitSpawnDataInfo>& UnitSpawnList = StageUnitDataAsset->UnitSpawnList;

    int32 RandomIndex = FMath::RandRange(0, UnitSpawnList.Num() - 1);

    TSubclassOf<AUnitCharacter> SelectedUnitClass;
    if (UnitSpawnList[RandomIndex].UnitData)
    {
        SelectedUnitClass = UnitSpawnList[RandomIndex].UnitData->EnemyClass;
    }

    if (SelectedUnitClass)
    {
        if (AUnitCharacter* DefaultUnit = SelectedUnitClass->GetDefaultObject<AUnitCharacter>())
        {
            // 이 유닛이 보스라면
            if (DefaultUnit->bIsBoss || DefaultUnit->bIsMiddleBoss)
            {
                if (bHasSpawnedBoss)
                {
                    UE_LOG(LogTemp, Warning, TEXT("이미나옴"));
                    int32 NextIndex = (RandomIndex + 1) % UnitSpawnList.Num();
                    if (UnitSpawnList[NextIndex].UnitData)
                    {
                        SelectedUnitClass = UnitSpawnList[NextIndex].UnitData->EnemyClass;
                    }
                    else
                    {
                        return; 
                    }
                }
                else
                {
                    bHasSpawnedBoss = true;
                    UE_LOG(LogTemp, Warning, TEXT("보스 스폰"));
                }
            }
        }

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
    SpawnInterval = 5.0f;
    bHasSpawnedBoss = false;

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
