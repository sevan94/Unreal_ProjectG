#include "Mode/Save/PGGameInstance.h"
#include "Mode/Save/PGSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "DataAssets/UI/UnitUIDataAsset.h"
#include "UI/Battle/BattleHUD.h"
#include "UI/DataTable/UnitUIDataTable.h"
#include "UI/DataTable/EquipUIDataTable.h"
#include "GameFramework/GameUserSettings.h"
#include "Sound/SoundClass.h"
#include "Sound/SoundMix.h"


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
        this->UnitLevelMap = CachedSaveData->UnitLevelMap;
        this->EquipMap = CachedSaveData->EquipMap;
    }
    else
    {
        CachedSaveData = Cast<UPGSaveGame>(UGameplayStatics::CreateSaveGameObject(UPGSaveGame::StaticClass()));
        bIsNewGame = true;
    }

    // 데이터가 비어있을 시 도감(Map) 초기화
    if (this->UnitLevelMap.IsEmpty()) InitializeUnitMap();
    if (this->EquipMap.IsEmpty()) InitializeEquipMap();

    if (bIsNewGame)
    {
        SetupDefaultSetting();
    }
    else
    {
        // 새 게임이 아닐 시
        // 유닛 데이터 로드
        CurrentUnits.Empty();
        for (const FSoftObjectPath& Path : CachedSaveData->EquippedUnitPaths)
        {
            CurrentUnits.Add(TSoftObjectPtr<UUnitUIDataAsset>(Path));
        }

        // 장비 로드
        CurrentWeapon = TSoftObjectPtr<UEquipUIDataAsset>(CachedSaveData->EquippedWeaponPath);
        CurrentArmor = TSoftObjectPtr<UEquipUIDataAsset>(CachedSaveData->EquippedArmorPath);
        CurrentAccessory = TSoftObjectPtr<UEquipUIDataAsset>(CachedSaveData->EquippedAccessoryPath);
    }

    // 디스크 데이터(Path) -> 런타임 데이터(SoftPtr) 로드
    
    // 플레이어 정보 로드 
    CurrentPlayerLevel = CachedSaveData->PlayerLevel;

    // 재화 로드
    CurrentPlayerGold = CachedSaveData->PlayerGold;
    CurrentPlayerGem = CachedSaveData->PlayerGem;
    CurrentPlayerUnlock = CachedSaveData->PlayerUnlock;

    // 클리어 스테이지 데이터 로드
    StageClearData = CachedSaveData->StageDataMap;

    // [환경설정] 사운드 설정 로드 (SaveGame에 이 변수들이 있어야 함)
    CurrentMasterVolume = CachedSaveData->MasterVolume;
    CurrentBGMVolume = CachedSaveData->BGMVolume;
    CurrentSFXVolume = CachedSaveData->SFXVolume;
    CurrentVoiceVolume = CachedSaveData->VoiceVolume;

    SetSoundVolumes(CurrentMasterVolume, CurrentBGMVolume, CurrentSFXVolume, CurrentVoiceVolume);

    if (bIsNewGame) SaveGameData();
}

void UPGGameInstance::AddGoods(EGoodsCategory InCategory, int32 InValue)
{
    int32 OutValue = 0;
    switch (InCategory)
    {
    case EGoodsCategory::Gem:
    {
        CurrentPlayerGem += InValue;
        OutValue = CurrentPlayerGem;
        break;
    }
    case EGoodsCategory::Unlock:
    {
        CurrentPlayerUnlock += InValue;
        OutValue = CurrentPlayerUnlock;
        break;
    }
    case EGoodsCategory::Gold:
    {
        CurrentPlayerGold += InValue;
        OutValue = CurrentPlayerGold;
        break;
    }
    }
    if (OutValue <= 0)
        OutValue = 0;
    if (OnGoodsChanged.IsBound())
    {
        OnGoodsChanged.Broadcast(InCategory, OutValue);
    }
}

void UPGGameInstance::ConsumeGoods(EGoodsCategory InCategory, int32 InValue)
{
    int32 OutValue = 0;
    switch (InCategory)
    {
    case EGoodsCategory::Gem:
    {
        CurrentPlayerGem -= InValue;
        if (CurrentPlayerGem < 0) CurrentPlayerGem = 0;
        OutValue = CurrentPlayerGem;
        break;
    }
    case EGoodsCategory::Unlock:
    {
        CurrentPlayerUnlock -= InValue;
        if (CurrentPlayerUnlock < 0) CurrentPlayerUnlock = 0;
        OutValue = CurrentPlayerUnlock;
        break;
    }
    case EGoodsCategory::Gold:
    {
        CurrentPlayerGold -= InValue;
        if (CurrentPlayerGold < 0) CurrentPlayerGold = 0;
        OutValue = CurrentPlayerGold;
        break;
    }
    }
    if (OnGoodsChanged.IsBound())
    {
        OnGoodsChanged.Broadcast(InCategory, OutValue);
    }
}

void UPGGameInstance::SetSoundVolumes(float InMaster, float InBGM, float InSFX, float InVoice)
{
    // 볼륨 값을 0.0 ~ 1.0 사이 설정
    CurrentMasterVolume = FMath::Clamp(InMaster, 0.0001f, 1.0f);
    CurrentBGMVolume = FMath::Clamp(InBGM, 0.0001f, 1.0f);
    CurrentSFXVolume = FMath::Clamp(InSFX, 0.0001f, 1.0f);
    CurrentVoiceVolume = FMath::Clamp(InVoice, 0.0001f, 1.0f);
    //UE_LOG(LogTemp, Log, TEXT("Master: %f, BGM: %f, SFX: %f"), InMaster, InBGM, InSFX);

    // 언리얼 엔진 사운드 믹스에 오버라이드 (적용)
    if (MainSoundMix)
    {
        UGameplayStatics::PushSoundMixModifier(this, MainSoundMix);
        if (MasterSoundClass) UGameplayStatics::SetSoundMixClassOverride(this, MainSoundMix, MasterSoundClass, CurrentMasterVolume, 1.0f, 0.0f, true);
        if (BGMSoundClass) UGameplayStatics::SetSoundMixClassOverride(this, MainSoundMix, BGMSoundClass, CurrentBGMVolume, 1.0f, 0.0f, true);
        if (SFXSoundClass) UGameplayStatics::SetSoundMixClassOverride(this, MainSoundMix, SFXSoundClass, CurrentSFXVolume, 1.0f, 0.0f, true);
        if (VoiceSoundClass) UGameplayStatics::SetSoundMixClassOverride(this, MainSoundMix, VoiceSoundClass, CurrentVoiceVolume, 1.0f, 0.0f, true);
    }

    // 볼륨이 바뀔 때마다 세이브 파일에 덮어쓰기
    SaveGameData();
}

void UPGGameInstance::SetScreenMode(int32 ModeIndex)
{
    if (UGameUserSettings* UserSettings = GEngine->GetGameUserSettings())
    {
        EWindowMode::Type NewMode = EWindowMode::Windowed;

        if (ModeIndex == 0) NewMode = EWindowMode::Fullscreen;               // 전체화면
        else if (ModeIndex == 1) NewMode = EWindowMode::WindowedFullscreen;  // 비율화면 (테두리 없는 창모드)
        else if (ModeIndex == 2) NewMode = EWindowMode::Windowed;            // 창모드

        UserSettings->SetFullscreenMode(NewMode);
        UserSettings->ApplySettings(false); // 해상도 및 모드 즉시 적용

        //.ini 파일에 즉시 기록하여 영구 저장
        UserSettings->SaveSettings();
    }
}

void UPGGameInstance::SetGraphicQuality(int32 QualityIndex)
{
    if (UGameUserSettings* UserSettings = GEngine->GetGameUserSettings())
    {
        // QualityIndex (0: 저, 1: 중, 2: 고)
        int32 SafeQuality = FMath::Clamp(QualityIndex, 0, 3);

        // 안티앨리어싱, 그림자, 텍스처 등 모든 품질을 일괄 변경
        UserSettings->SetOverallScalabilityLevel(SafeQuality);
        UserSettings->ApplySettings(false);

        //.ini 파일에 즉시 기록하여 영구 저장
        UserSettings->SaveSettings();
    }
}

void UPGGameInstance::UpdateStageClearData(int32 StageCode, int32 InStarCount)
{
    int32* OldStarCountPtr = StageClearData.Find(StageCode);
    bool bIsFirstClear = (OldStarCountPtr == nullptr);

    // 젬은 최초 클리어시에만 지급
    if (bIsFirstClear)
    {
        AddGoods(EGoodsCategory::Gem, CurrentStageData.RewardGem);
    }
    
    // 달성도에 따라 골드 지급
    AddGoods(EGoodsCategory::Gold, CurrentStageData.RewardGold * InStarCount);

    // 처음 클리어했거나, 이전보다 더 높은 별점을 획득했을 때 기록 갱신
    if (bIsFirstClear || InStarCount > *OldStarCountPtr)
    {
        StageClearData.FindOrAdd(StageCode) = InStarCount;
    }

    SaveGameData();
}

void UPGGameInstance::InitializeUnitMap()
{
    if (!UnitDataTable) return;

    // 이미 데이터가 있다면 중복 초기화를 방지
    //if (UnitLevelMap.Num() > 0) return;

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

            // 특정 ID만 시작 시 해금 상태로 설정
            NewData.bIsUnlocked = (
                Row->UnitID == 101 ||
                Row->UnitID == 102 ||
                Row->UnitID == 201 ||
                Row->UnitID == 204 ||
                Row->UnitID == 303 ||
                Row->UnitID == 401
                );

            // 맵에 추가
            UnitLevelMap.Add(Row->UnitID, NewData);
        }
    }

    UE_LOG(LogTemp, Log, TEXT("UnitLevelMap 초기화 완료: %d개의 유닛 로드됨"), UnitLevelMap.Num());
}

void UPGGameInstance::InitializeEquipMap()
{
    // 이미 데이터가 있다면 중복 초기화 방지
    if (EquipMap.Num() > 0) return;

    // 순회할 데이터 테이블 배열 (에디터에서 할당된 변수들)
    TArray<UDataTable*> TargetTables;
    if (WeaponDataTable) TargetTables.Add(WeaponDataTable);
    if (ArmorDataTable) TargetTables.Add(ArmorDataTable);
    if (AccessoryDataTable) TargetTables.Add(AccessoryDataTable);

    static const FString ContextString(TEXT("EquipMapRef"));

    for (UDataTable* Table : TargetTables)
    {
        if (!Table) continue;

        // FEquipUIDataTable 형식으로 모든 행을 가져옴
        TArray<FEquipUIDataTable*> AllRows;
        Table->GetAllRows<FEquipUIDataTable>(ContextString, AllRows);

        for (FEquipUIDataTable* Row : AllRows)
        {
            if (Row)
            {
                // 특정 ID만 시작 시 해금 상태로 설정
                bool bIsUnlocked = (
                    Row->EquipID == 1001 || // 기본 무기
                    Row->EquipID == 1004 || // 기본 무기
                    Row->EquipID == 2001 || // 기본 방어구
                    Row->EquipID == 2004 || // 기본 방어구
                    Row->EquipID == 3001 || // 기본 악세서리
                    Row->EquipID == 3004    // 기본 악세서리
                    );

                // EquipMap에 추가
                if (!EquipMap.Contains(Row->EquipID))
                {
                    EquipMap.Add(Row->EquipID, bIsUnlocked);
                }
            }
        }
    }

    UE_LOG(LogTemp, Log, TEXT("EquipMap 초기화 완료: 총 %d개의 장비 로드됨"), EquipMap.Num());
}

void UPGGameInstance::SetupDefaultSetting()
{
    // --- 유닛 초기 장착 ---
    if (UnitDataTable)
    {
        static const FString ContextString(TEXT("DefaultUnitSetup"));
        TArray<FUnitUIDataTable*> Rows;
        UnitDataTable->GetAllRows<FUnitUIDataTable>(ContextString, Rows);

        for (FUnitUIDataTable* Row : Rows)
        {
            if (Row->UnitID == 101 ||
                Row->UnitID == 102 ||
                Row->UnitID == 201 ||
                Row->UnitID == 202 ||
                Row->UnitID == 301
                )
            {
                CurrentUnits.Add(TSoftObjectPtr<UUnitUIDataAsset>(Row->UnitData));
            }
        }
    }

    // -- - 장비 초기 장착-- -
    TArray<UDataTable*> Tables = { WeaponDataTable, ArmorDataTable, AccessoryDataTable };
    for (UDataTable* Table : Tables)
    {
        if (!Table) continue;
        TArray<FEquipUIDataTable*> Rows;
        Table->GetAllRows<FEquipUIDataTable>(TEXT("DefaultEquipSetup"), Rows);

        for (FEquipUIDataTable* Row : Rows)
        {
            if (Row->EquipID == 1001) CurrentWeapon = TSoftObjectPtr<UEquipUIDataAsset>(Row->EquipData);
            if (Row->EquipID == 2001) CurrentArmor = TSoftObjectPtr<UEquipUIDataAsset>(Row->EquipData);
            if (Row->EquipID == 3002) CurrentAccessory = TSoftObjectPtr<UEquipUIDataAsset>(Row->EquipData);
        }
    }
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

bool UPGGameInstance::GetEquipOwned(int32 EquipID)
{
    if (bool* bIsOwned = EquipMap.Find(EquipID))
    {
        return *bIsOwned;
    }
    return false;
}

void UPGGameInstance::SaveGameData()
{
    if (!CachedSaveData) return;

    // 플레이어 레벨 저장
    CachedSaveData->PlayerLevel = this->CurrentPlayerLevel;

    // 현재 보유 유닛 리스트 저장
    CachedSaveData->UnitLevelMap = this->UnitLevelMap;

    // 현재 보유 장비 리스트 저장
    CachedSaveData->EquipMap = this->EquipMap;

    // 현재 스테이지 데이터 저장
    CachedSaveData->StageDataMap = this->StageClearData;

    // 런타임 데이터(SoftPtr) -> 디스크 데이터(Path) 저장
    // 게임 중 변동된 장비를 세이브 파일에 덮어쓰기
    CachedSaveData->EquippedWeaponPath = CurrentWeapon.ToSoftObjectPath();
    CachedSaveData->EquippedArmorPath = CurrentArmor.ToSoftObjectPath();
    CachedSaveData->EquippedAccessoryPath = CurrentAccessory.ToSoftObjectPath();

    // 게임 중 변동된 재화를 세이브 파일에 덮어쓰기
    CachedSaveData->PlayerGold = CurrentPlayerGold;
    CachedSaveData->PlayerGem = CurrentPlayerGem;
    CachedSaveData->PlayerUnlock = CurrentPlayerUnlock;

    // [환경설정] 사운드 설정 디스크에 저장
    CachedSaveData->MasterVolume = CurrentMasterVolume;
    CachedSaveData->BGMVolume = CurrentBGMVolume;
    CachedSaveData->SFXVolume = CurrentSFXVolume;

    CachedSaveData->EquippedUnitPaths.Empty();
    for (const auto& UnitPtr : CurrentUnits)
    {
        CachedSaveData->EquippedUnitPaths.Add(UnitPtr.ToSoftObjectPath());
    }

    UGameplayStatics::SaveGameToSlot(CachedSaveData, SaveSlotName, 0);
}