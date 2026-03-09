#include "Mode/Save/PGGameInstance.h"
#include "Mode/Save/PGSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "DataAssets/UI/UnitUIDataAsset.h"
#include "UI/Battle/BattleHUD.h"
#include "UI/DataTable/UnitUIDataTable.h"


//------- 구현 방식 ----------
//세이브 및 인스턴스 구현 방식: UI 버튼 클릭 이벤트 함수 내부 어딘가
//if (UPGGameInstance* GI = Cast<UPGGameInstance>(GetGameInstance()))
//{
//    GI->CurrentWeapon = SelectedWeapon; // 1. 바꾼 무기 적용
//    GI->SaveGameData();                 // 2. 즉시 디스크에 자동 저장!
//}

// PGBaseGameMode.cpp 의 OnGameOver 함수 내부
//if (bIsPlayerVictory)
//{
//    // ... 별점 계산 및 골드 지급 로직 ...
//
//    if (UPGGameInstance* GI = Cast<UPGGameInstance>(GetGameInstance()))
//    {
//        GI->SaveGameData(); // 획득한 골드와 진행도를 자동 저장!
//    }
//}

void UPGGameInstance::Init()
{
    Super::Init();

    LoadGameData();
}

void UPGGameInstance::LoadGameData()
{
    // 세이브 파일이 있으면 불러오고, 없으면 새로 생성
    if (UGameplayStatics::DoesSaveGameExist(SaveSlotName, 0))
    {
        CachedSaveData = Cast<UPGSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0));
        InitializeUnitMap();
    }
    else
    {
        CachedSaveData = Cast<UPGSaveGame>(UGameplayStatics::CreateSaveGameObject(UPGSaveGame::StaticClass()));
    }

    // 디스크 데이터(Path) -> 런타임 데이터(SoftPtr) 로드
    //장비
    CurrentWeapon = TSoftObjectPtr<UEquipUIDataAsset>(CachedSaveData->EquippedWeaponPath);
    CurrentArmor = TSoftObjectPtr<UEquipUIDataAsset>(CachedSaveData->EquippedArmorPath);
    CurrentAccessory = TSoftObjectPtr<UEquipUIDataAsset>(CachedSaveData->EquippedAccessoryPath);

    //재화
    CurrentPlayerGold = CachedSaveData->PlayerGold;
    CurrentPlayerGem = CachedSaveData->PlayerGem;
    CurrentPlayerUnlock = CachedSaveData->PlayerUnlock;

    CurrentUnits.Empty();
    for (const FSoftObjectPath& Path : CachedSaveData->EquippedUnitPaths)
    {
        CurrentUnits.Add(TSoftObjectPtr<UUnitUIDataAsset>(Path));
    }
}

void UPGGameInstance::InitializeUnitMap()
{
    if (!UnitDataTable) return;

    // 이미 데이터가 있다면 중복 초기화를 방지
    if (UnitLevelMap.Num() > 0) return;

    // 데이터 테이블의 모든 행을 가져옴 (RowStruct는 본인의 데이터 테이블 구조체 타입)
    static const FString ContextString(TEXT("UnitMapRef"));
    TArray<FUnitUIDataTable*> AllRows;
    UnitDataTable->GetAllRows<FUnitUIDataTable>(ContextString, AllRows);

    for (FUnitUIDataTable* Row : AllRows)
    {
        if (Row)
        {
            FUnitSaveData NewData;
            NewData.Level = 1;

            // 특정 ID(예: 101번 전사)만 시작 시 해금 상태로 설정
            NewData.bIsUnlocked = (Row->UnitID == 101);

            // 맵에 추가
            UnitLevelMap.Add(Row->UnitID, NewData);
        }
    }

    UE_LOG(LogTemp, Log, TEXT("UnitLevelMap 초기화 완료: %d개의 유닛 로드됨"), UnitLevelMap.Num());
}

FUnitSaveData UPGGameInstance::GetUnitSaveData(int32 UnitID)
{
    if (FUnitSaveData* FoundData = UnitLevelMap.Find(UnitID))
    {
        // 데이터 존재 시 역참조
        return *FoundData;
    }
    return FUnitSaveData();
}

void UPGGameInstance::SaveGameData()
{
    if (!CachedSaveData) return;

    // 런타임 데이터(SoftPtr) -> 디스크 데이터(Path) 저장
    // 게임 중 변동된 장비를 세이브 파일에 덮어쓰기
    CachedSaveData->EquippedWeaponPath = CurrentWeapon.ToSoftObjectPath();
    CachedSaveData->EquippedArmorPath = CurrentArmor.ToSoftObjectPath();
    CachedSaveData->EquippedAccessoryPath = CurrentAccessory.ToSoftObjectPath();

    // 게임 중 변동된 재화를 세이브 파일에 덮어쓰기
    CachedSaveData->PlayerGold = CurrentPlayerGold;
    CachedSaveData->PlayerGem = CurrentPlayerGem;
    CachedSaveData->PlayerUnlock = CurrentPlayerUnlock;

    CachedSaveData->EquippedUnitPaths.Empty();
    for (const auto& UnitPtr : CurrentUnits)
    {
        CachedSaveData->EquippedUnitPaths.Add(UnitPtr.ToSoftObjectPath());
    }

    UGameplayStatics::SaveGameToSlot(CachedSaveData, SaveSlotName, 0);
}