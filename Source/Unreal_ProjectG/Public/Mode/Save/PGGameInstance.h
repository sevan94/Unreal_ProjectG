// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Character/Unit/UnitCharacter.h"
#include "Types/PGEnumTypes.h"
#include "UI/DataTable/StageDataTable.h"
#include "PGGameInstance.generated.h"

class UPGSaveGame;
class UUnitUIDataAsset;
class UEquipUIDataAsset;
class AHeroCharacter;
class UGameUserSettings;
class USoundMix;
class USoundClass;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnGoodsChanged, EGoodsCategory, InCategory, int32, InValue);

USTRUCT(BlueprintType)
struct FUnitSaveData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Level = 1;        // 유닛 레벨

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsUnlocked = false; // 보유 여부
};

UCLASS()
class UNREAL_PROJECTG_API UPGGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
    virtual void Init() override;

    // --- [Save/Load 시스템] ---
    UFUNCTION(BlueprintCallable, Category = "SaveGame")
    void SaveGameData();

    UFUNCTION(BlueprintCallable, Category = "SaveGame")
    void LoadGameData();

    // 재화 추가
    UFUNCTION(BlueprintCallable, Category = "SaveGame")
    void AddGoods(EGoodsCategory InCategory, int32 InValue);

    // 재화 소비
    UFUNCTION(BlueprintCallable, Category = "SaveGame")
    void ConsumeGoods(EGoodsCategory InCategory, int32 InValue);

    // ==========================================================
    // --- [환경설정 (옵션) 시스템] ---
    // ==========================================================
    // 1. 사운드 볼륨 조절 (0.0 ~ 1.0)
    UFUNCTION(BlueprintCallable, Category = "Settings|Sound")
    void SetSoundVolumes(float InMaster, float InBGM, float InSFX, float InVoice);

    // 2. 화면 모드 설정 (0: 전체화면, 1: 비율(테두리없는)화면, 2: 창모드)
    UFUNCTION(BlueprintCallable, Category = "Settings|Display")
    void SetScreenMode(int32 ModeIndex);

    // 3. 그래픽 품질 설정 (0: 저, 1: 중, 2: 고)
    UFUNCTION(BlueprintCallable, Category = "Settings|Graphic")
    void SetGraphicQuality(int32 QualityIndex);

    // 스테이지 결과 정보 저장
    void UpdateStageClearData(int32 StageCode, int32 InStarCount);

    // 선택한 스테이지 데이터
    inline void SetCurrentStageData(const FStageDataTable& InData) { CurrentStageData = InData; }

    // 스테이지의 달성도를 리턴하는 함수
    inline int32 GetStageStarCount(int32 StageCode)
    {
        if (StageClearData.Contains(StageCode))
        {
            return StageClearData[StageCode];
        }
        return 0; // 데이터가 없으면 미클리어 상태
    }
    
    // 뽑기를 위한 유닛 데이터 테이블을 리턴하는 함수
    UDataTable* GetUnitDataTable() { return UnitDataTable; }
    UDataTable* GetWeaponDataTable() { return WeaponDataTable; }
    UDataTable* GetArmorDataTable() { return ArmorDataTable; }
    UDataTable* GetAccessoryDataTable() { return AccessoryDataTable; }

protected:
    UFUNCTION(BlueprintCallable, Category = "SaveData")
    void InitializeUnitMap();

    UFUNCTION(BlueprintCallable, Category = "SaveData")
    void InitializeEquipMap();

    void SetupDefaultSetting();

public:
    FUnitSaveData GetUnitSaveData(int32 UnitID);
    bool GetEquipOwned(int32 EquipID);

    // --- [런타임 장착 데이터] ---
    // 로비 UI에서 이 변수들을 수정하고, 전투 맵에서 이 변수들을 읽어 적용
    UPROPERTY(BlueprintReadWrite, Category = "Equipment")
    TSoftObjectPtr<UEquipUIDataAsset> CurrentWeapon;

    UPROPERTY(BlueprintReadWrite, Category = "Equipment")
    TSoftObjectPtr<UEquipUIDataAsset> CurrentArmor;

    UPROPERTY(BlueprintReadWrite, Category = "Equipment")
    TSoftObjectPtr<UEquipUIDataAsset> CurrentAccessory;

    UPROPERTY(BlueprintReadWrite, Category = "Equipment")
    TArray<TSoftObjectPtr<UUnitUIDataAsset>> CurrentUnits;

    UPROPERTY(BlueprintReadWrite, Category = "Equipment")
    FGameplayTag CurrentSetTag = FGameplayTag::EmptyTag;

    // 로비에서 선택한 스테이지 정보
    UPROPERTY(BlueprintReadWrite, Category = "Stage")
    FStageDataTable CurrentStageData;

    // 스테이지 데이터 정보 (스테이지 코드, 스테이지 달성도 정보)
    UPROPERTY(BlueprintReadWrite, Category = "SaveData")
    TMap<int32, int32> StageClearData;

    // 플레이어 레벨
    UPROPERTY(BlueprintReadWrite, Category = "Player Info")
    int32 CurrentPlayerLevel;

    // 소지 재화
    UPROPERTY(BlueprintReadWrite, Category = "Player Info")
    int32 CurrentPlayerGold;

    UPROPERTY(BlueprintReadWrite, Category = "Player Info")
    int32 CurrentPlayerGem;

    UPROPERTY(BlueprintReadWrite, Category = "Player Info")
    int32 CurrentPlayerUnlock;

    // 현재 선택 스테이지
    UPROPERTY(BlueprintReadWrite, Category = "Player Info")
    int32 CurrentStageIndex = 0;

    UPROPERTY(BlueprintReadWrite, Category = "Lobby")
    bool bStageSelect = false;

    // 유닛 도감 형식 데이터(유닛 코드, 유닛 데이터)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SaveData")
    TMap<int32, FUnitSaveData> UnitLevelMap;

    // 장비 도감 형식 데이터(장비 코드, 보유 현황)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SaveData")
    TMap<int32, bool> EquipMap;

    FOnGoodsChanged OnGoodsChanged;

    // --- [환경설정 런타임 변수 (사운드)] ---
    UPROPERTY(BlueprintReadWrite, Category = "Settings|Sound")
    float CurrentMasterVolume = 1.0f;
    UPROPERTY(BlueprintReadWrite, Category = "Settings|Sound")
    float CurrentBGMVolume = 1.0f;
    UPROPERTY(BlueprintReadWrite, Category = "Settings|Sound")
    float CurrentSFXVolume = 1.0f;
    UPROPERTY(BlueprintReadWrite, Category = "Settings|Sound")
    float CurrentVoiceVolume = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Sound")
    TObjectPtr<USoundMix> MainSoundMix;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Sound")
    TObjectPtr<USoundClass> MasterSoundClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Sound")
    TObjectPtr<USoundClass> BGMSoundClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Sound")
    TObjectPtr<USoundClass> SFXSoundClass;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Sound")
    TObjectPtr<USoundClass> VoiceSoundClass;


protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
    TObjectPtr<UDataTable> UnitDataTable;

    // 장비 데이터 테이블들
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
    TObjectPtr<UDataTable> WeaponDataTable;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
    TObjectPtr<UDataTable> ArmorDataTable;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
    TObjectPtr<UDataTable> AccessoryDataTable;

private:
    UPROPERTY()
    TObjectPtr<UPGSaveGame> CachedSaveData;

    FString SaveSlotName = TEXT("PG_SaveSlot_01");

    // 최초 게임 시작시 기본값 세팅을 위한 변수
    bool bIsNewGame = false;
};
