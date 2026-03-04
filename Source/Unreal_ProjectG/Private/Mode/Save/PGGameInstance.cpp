#include "Mode/Save/PGGameInstance.h"
#include "Mode/Save/PGSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "DataAssets/Items/DataAsset_WeaponData.h"
#include "DataAssets/Items/DataAsset_ArmorData.h"
#include "DataAssets/Items/DataAsset_AccessoryData.h"
#include "DataAssets/UI/UnitUIDataAsset.h"
#include "UI/Battle/BattleHUD.h"


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
    }
    else
    {
        CachedSaveData = Cast<UPGSaveGame>(UGameplayStatics::CreateSaveGameObject(UPGSaveGame::StaticClass()));
    }

    // 디스크 데이터(Path) -> 런타임 데이터(SoftPtr) 로드
    //장비
    CurrentWeapon = TSoftObjectPtr<UDataAsset_WeaponData>(CachedSaveData->EquippedWeaponPath);
    CurrentArmor = TSoftObjectPtr<UDataAsset_ArmorData>(CachedSaveData->EquippedArmorPath);
    CurrentAccessory = TSoftObjectPtr<UDataAsset_AccessoryData>(CachedSaveData->EquippedAccessoryPath);

    //재화
    CurrentPlayerGold = CachedSaveData->PlayerGold;
    CurrentPlayerRuby = CachedSaveData->PlayerRuby;
    CurrentPlayerPiece = CachedSaveData->PlayerPiece;

    CurrentUnits.Empty();
    for (const FSoftObjectPath& Path : CachedSaveData->EquippedUnitPaths)
    {
        CurrentUnits.Add(TSoftObjectPtr<UUnitUIDataAsset>(Path));
    }
}

void UPGGameInstance::InitBattleUI(AHeroCharacter* Hero, APlayerController* PC)
{
    if (ABattleHUD* HUD = Cast<ABattleHUD>(PC->GetHUD()))
    {
        if (HUD)
        {
            // 스펙 핸들 전달
            HUD->SetAbilitySpec();
        }
    }
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
    CachedSaveData->PlayerRuby = CurrentPlayerRuby;
    CachedSaveData->PlayerPiece = CurrentPlayerPiece;

    CachedSaveData->EquippedUnitPaths.Empty();
    for (const auto& UnitPtr : CurrentUnits)
    {
        CachedSaveData->EquippedUnitPaths.Add(UnitPtr.ToSoftObjectPath());
    }

    UGameplayStatics::SaveGameToSlot(CachedSaveData, SaveSlotName, 0);
}