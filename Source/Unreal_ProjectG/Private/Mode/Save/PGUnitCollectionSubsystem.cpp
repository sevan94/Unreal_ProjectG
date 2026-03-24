// Fill out your copyright notice in the Description page of Project Settings.

#include "Mode/Save/PGUnitCollectionSubsystem.h"
#include "DataAssets/UI/UnitUIDataAsset.h"
#include "Mode/Save/PGGameInstance.h"
#include "UI/DataTable/UnitUIDataTable.h"


// -------------------------------------------------------------------
// [1. 뽑기 (가챠) 굴리기 로직]
// -------------------------------------------------------------------
UUnitUIDataAsset* UPGUnitCollectionSubsystem::RollSingleGacha()
{
    UPGGameInstance* GI = Cast<UPGGameInstance>(GetGameInstance());
    if (!GI || !GI->GetUnitDataTable()) return nullptr;

    // DataTable에서 모든 행(Row)을 가져옴
    TArray<FUnitUIDataTable*> AllUnitRows;
    GI->GetUnitDataTable()->GetAllRows<FUnitUIDataTable>(TEXT("GachaRoll"), AllUnitRows);

    if (AllUnitRows.Num() == 0) return nullptr;

    // 랜덤 인덱스 선택
    int32 RandomIndex = FMath::RandRange(0, AllUnitRows.Num() - 1);
    FUnitUIDataTable* SelectedRow = AllUnitRows[RandomIndex];

    UUnitUIDataAsset* PickedUnit = SelectedRow->UnitData;
    if (PickedUnit)
    {
        ProcessGachaResult(PickedUnit);
    }

    return PickedUnit;
}

TArray<UUnitUIDataAsset*> UPGUnitCollectionSubsystem::RollMultiGacha(const TArray<UUnitUIDataAsset*>& GachaPool, int32 PullCount)
{
    TArray<UUnitUIDataAsset*> Results;
    for (int32 i = 0; i < PullCount; ++i)
    {
        if (UUnitUIDataAsset* PickedUnit = RollSingleGacha())
        {
            Results.Add(PickedUnit);
        }
    }
    return Results;
}

// -------------------------------------------------------------------
// [2. 뽑기 결과 처리 및 GameInstance 데이터 갱신]
// -------------------------------------------------------------------
void UPGUnitCollectionSubsystem::ProcessGachaResult(UUnitUIDataAsset* PickedUnitData)
{
    if (!PickedUnitData) return;

    UPGGameInstance* GI = Cast<UPGGameInstance>(GetGameInstance());
    if (!GI) return;

    int32 PickedID = PickedUnitData->UnitID;

    // GameInstance의 유닛 맵을 확인하여 이미 보유 중인지 검사
    if (IsUnitOwned(PickedID))
    {
        // [중복 획득] 보상량 가져오기 (UnitUIDataAsset에 추가한 변수를 가져옴 )
        int32 RewardToGive = PickedUnitData->DuplicateUnitReward;

        // AddGoods 델리게이트 시스템 호출 
        // 이렇게 하면 CurrentPlayerUnlock이 오르면서 UI(상단바 등)도 자동 갱신
        GI->AddGoods(EGoodsCategory::Unlock, RewardToGive);

        UE_LOG(LogTemp, Log, TEXT("[%s] 중복 획득! 조각 %d개 지급. (현재 총 조각: %d)"),
            *PickedUnitData->GetName(), RewardToGive, GI->CurrentPlayerUnlock);
    }
    else
    {
        // [신규 획득] GameInstance의 UnitLevelMap 데이터 수정
        if (FUnitSaveData* FoundData = GI->UnitLevelMap.Find(PickedID))
        {
            FoundData->bIsUnlocked = true; // 잠금 해제
        }
        else
        {
            // 만약 맵에 아예 등록조차 안 된 유닛이라면 새로 등록
            FUnitSaveData NewData;
            NewData.Level = 1;
            NewData.bIsUnlocked = true;
            GI->UnitLevelMap.Add(PickedID, NewData);
        }

        // 도감 UI 등에서 자물쇠를 풀 수 있도록 이벤트 방송
        //OnGachaUnitUnlocked.Broadcast(PickedUnitData);

        UE_LOG(LogTemp, Log, TEXT("[%s] 신규 획득! 도감 등록 완료"), *PickedUnitData->GetName());
    }

    // 결과 처리 후 즉시 세이브 파일에 기록
    GI->SaveGameData();
}

// -------------------------------------------------------------------
// [3. 보유/레벨 조회 로직 (GameInstance 직접 참조) ]
// -------------------------------------------------------------------
bool UPGUnitCollectionSubsystem::IsUnitOwned(int32 UnitID) const
{
    UPGGameInstance* GI = Cast<UPGGameInstance>(GetGameInstance());
    if (GI)
    {
        FUnitSaveData Data = GI->GetUnitSaveData(UnitID);
        return Data.bIsUnlocked;
    }
    return false;
}

int32 UPGUnitCollectionSubsystem::GetUnitLevel(int32 UnitID) const
{
    UPGGameInstance* GI = Cast<UPGGameInstance>(GetGameInstance());
    if (GI)
    {
        FUnitSaveData Data = GI->GetUnitSaveData(UnitID);
        return Data.Level;
    }
    return 1;
}