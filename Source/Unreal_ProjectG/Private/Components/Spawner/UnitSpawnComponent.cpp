
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
    if (!StageUnitDataAsset || StageUnitDataAsset->UnitSpawnList.Num() == 0)
    {
        return;
    }

    const TArray<FUnitSpawnDataInfo>& UnitSpawnList = StageUnitDataAsset->UnitSpawnList; //

    TArray<int32> ValidIndices;
    int32 TotalWeight = 0;

    // 1. 스폰 가능한 유닛들만 추려서 인덱스와 총 가중치를 계산합니다.
    for (int32 i = 0; i < UnitSpawnList.Num(); ++i)
    {
        const FUnitSpawnDataInfo& UnitInfo = UnitSpawnList[i];

        if (UnitInfo.UnitData && UnitInfo.UnitData->EnemyClass)
        {
            if (AUnitCharacter* DefaultUnit = UnitInfo.UnitData->EnemyClass->GetDefaultObject<AUnitCharacter>())
            {
                // 핵심: 유닛이 보스(또는 중간 보스)인데 이미 스폰되었다면, 이번 추첨 목록에서 아예 제외합니다.
                if ((DefaultUnit->bIsBoss || DefaultUnit->bIsMiddleBoss) && bHasSpawnedBoss) //
                {
                    continue;
                }

                // 가중치가 0보다 큰 정상적인 유닛만 추첨 목록에 넣습니다.
                if (UnitInfo.Spawnweight > 0)
                {
                    ValidIndices.Add(i);
                    TotalWeight += UnitInfo.Spawnweight;
                }
            }
        }
    }

    // 스폰할 수 있는 유닛이 없거나 가중치 총합이 0이면 스폰을 중단합니다.
    if (ValidIndices.Num() == 0 || TotalWeight <= 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("스폰 가능한 유닛이 없거나 총 가중치가 0입니다."));
        return;
    }

    // 2. 가중치 기반 랜덤 인덱스 선택
    int32 RandomWeight = FMath::RandRange(1, TotalWeight);
    int32 SelectedIndex = -1;
    int32 AccumulatedWeight = 0;

    for (int32 ValidIndex : ValidIndices)
    {
        AccumulatedWeight += UnitSpawnList[ValidIndex].Spawnweight;

        if (RandomWeight <= AccumulatedWeight)
        {
            SelectedIndex = ValidIndex;
            break;
        }
    }

    if (SelectedIndex == -1) return; // 안전장치

    // 3. 최종 선택된 유닛 정보 가져오기
    TSubclassOf<AUnitCharacter> SelectedUnitClass = UnitSpawnList[SelectedIndex].UnitData->EnemyClass;

    // 방금 뽑힌 유닛이 보스라면, 다음 스폰부터는 제외되도록 상태를 업데이트합니다.
    if (AUnitCharacter* DefaultUnit = SelectedUnitClass->GetDefaultObject<AUnitCharacter>())
    {
        if (DefaultUnit->bIsBoss || DefaultUnit->bIsMiddleBoss) //
        {
            bHasSpawnedBoss = true; //
            UE_LOG(LogTemp, Warning, TEXT("보스 스폰")); //
        }
    }

    // 4. 유닛 스폰 실행
    if (UWorld* World = GetWorld()) //
    {
        if (UUnitSpawnSubsystem* SpawnSystem = World->GetSubsystem<UUnitSpawnSubsystem>()) //
        {
            SpawnSystem->SpawnUnit( //
                SelectedUnitClass, //
                GetComponentLocation(), //
                GetComponentRotation(), //
                AttackTarget //
            );
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
