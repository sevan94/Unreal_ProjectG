// Fill out your copyright notice in the Description page of Project Settings.

#include "Mode/Save/PGEquipCollectionSubsystem.h"
#include "DataAssets/UI/EquipUIDataAsset.h"
#include "Mode/Save/PGGameInstance.h"
#include "UI/DataTable/EquipUIDataTable.h"


UEquipUIDataAsset* UPGEquipCollectionSubsystem::RollSingleGacha()
{
    UPGGameInstance* GI = Cast<UPGGameInstance>(GetGameInstance());
    // GameInstance 및 테이블 존재 확인 (Weapon, Armor, Accessory 중 하나라도 있어야 함)
    if (!GI) return nullptr;

    // 모든 장비 테이블에서 통합 리스트 작성
    TArray<UDataTable*> TargetTables;
    TargetTables.Add(GI->GetWeaponDataTable());
    TargetTables.Add(GI->GetArmorDataTable());
    TargetTables.Add(GI->GetAccessoryDataTable());

    TArray<FEquipUIDataTable*> AllEquipRows;
    static const FString ContextString(TEXT("GachaRoll"));

    for (UDataTable* Table : TargetTables)
    {
        if (Table)
        {
            TArray<FEquipUIDataTable*> TempRows;
            Table->GetAllRows<FEquipUIDataTable>(ContextString, TempRows);
            AllEquipRows.Append(TempRows);
        }
    }

    if (AllEquipRows.Num() == 0) return nullptr;

    // 랜덤 인덱스 선택
    int32 RandomIndex = FMath::RandRange(0, AllEquipRows.Num() - 1);
    FEquipUIDataTable* SelectedRow = AllEquipRows[RandomIndex];

    UEquipUIDataAsset* PickedEquip = SelectedRow->EquipData;
    if (PickedEquip)
    {
        ProcessGachaResult(PickedEquip);
    }

    return PickedEquip;
}

void UPGEquipCollectionSubsystem::ProcessGachaResult(UEquipUIDataAsset* PickedEquipData)
{
    if (!PickedEquipData) return;

    UPGGameInstance* GI = Cast<UPGGameInstance>(GetGameInstance());
    if (!GI) return;

    int32 PickedID = PickedEquipData->EquipID;

    // 보유 여부 확인
    if (IsEquipOwned(PickedID))
    {
        // [중복 획득] 보상 지급 (조각 등)
        int32 RewardToGive = PickedEquipData->DuplicateEquipReward;
        GI->AddGoods(EGoodsCategory::Unlock, RewardToGive);

        UE_LOG(LogTemp, Log, TEXT("[장비 중복] %s - 조각 %d개 지급"), *PickedEquipData->GetName(), RewardToGive);
    }
    else
    {
        // [신규 획득] EquipMap 업데이트 (단순 bool값 변경)
        GI->EquipMap.Add(PickedID, true);

        UE_LOG(LogTemp, Log, TEXT("[장비 신규] %s - 도감 등록 완료"), *PickedEquipData->GetName());
    }

    // 결과 즉시 저장
    GI->SaveGameData();
}

bool UPGEquipCollectionSubsystem::IsEquipOwned(int32 EquipID) const
{
    UPGGameInstance* GI = Cast<UPGGameInstance>(GetGameInstance());
    if (GI)
    {
        return GI->GetEquipOwned(EquipID);
    }
    return false;
}