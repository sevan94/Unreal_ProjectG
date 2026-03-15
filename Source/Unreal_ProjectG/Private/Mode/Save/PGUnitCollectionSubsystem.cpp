#include "Mode/Save/UnitCollectionSubsystem.h"
#include "DataAssets/UI/UnitUIDataAsset.h"
#include "Mode/Save/PGGameInstance.h"
#include "Mode/Save/PGSaveGame.h"

void UPGUnitCollectionSubsystem::Deinitialize()
{
    OwnedUnits.Empty();
    Super::Deinitialize();
}

void UPGUnitCollectionSubsystem::ProcessGachaResult(UUnitUIDataAsset* PickedUnitData)
{
    if (!PickedUnitData) return;

    UPGGameInstance* GI = Cast<UPGGameInstance>(GetGameInstance());
    if (!GI) return;

    // 1. 중복 검사
    if (IsUnitOwned(PickedUnitData))
    {
        // [중복 획득] 기획자가 데이터 에셋에 설정한 조각 재화량
        int32 RewardToGive = PickedUnitData->DuplicateRewardAmount;

        GI->CurrentPlayerUnlock += RewardToGive;

        UE_LOG(LogTemp, Log, TEXT("[%s] 중복 획득! 조각 %d개 지급. (현재 총 조각: %d)"),
            *PickedUnitData->UnitName.ToString(), RewardToGive, GI->CurrentPlayerUnlock);
    }
    else
    {
        // [신규 획득] 도감 배열에 추가
        OwnedUnits.Add(PickedUnitData);
        OnUnitUnlocked.Broadcast(PickedUnitData);

        UE_LOG(LogTemp, Log, TEXT("[%s] 신규 획득! 도감 등록 완료"), *PickedUnitData->UnitName.ToString());
    }

    // 2. 결과 즉시 저장
    GI->SaveGameData();
}

bool UPGUnitCollectionSubsystem::IsUnitOwned(UUnitUIDataAsset* UnitData) const
{
    return OwnedUnits.Contains(UnitData);
}

TArray<UUnitUIDataAsset*> UPGUnitCollectionSubsystem::GetOwnedUnits() const
{
    return OwnedUnits;
}

void UPGUnitCollectionSubsystem::LoadFromSaveGame(const UPGSaveGame* SaveGame)
{
    if (!SaveGame) return;

    OwnedUnits.Empty();

    for (const FSoftObjectPath& Path : SaveGame->OwnedUnitPaths)
    {
        if (Path.IsValid())
        {
            if (UUnitUIDataAsset* LoadedUnit = Cast<UUnitUIDataAsset>(Path.TryLoad()))
            {
                OwnedUnits.Add(LoadedUnit);
            }
        }
    }
}

void UPGUnitCollectionSubsystem::SaveToSaveGame(UPGSaveGame* SaveGame) const
{
    if (!SaveGame) return;

    SaveGame->OwnedUnitPaths.Empty();
    for (UUnitUIDataAsset* Unit : OwnedUnits)
    {
        if (Unit) SaveGame->OwnedUnitPaths.Add(FSoftObjectPath(Unit));
    }
}