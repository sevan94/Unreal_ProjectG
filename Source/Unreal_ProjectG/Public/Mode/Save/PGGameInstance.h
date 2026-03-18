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

    // 스테이지 결과 정보 저장
    void SaveStageClearProgress(int32 StageCode, int32 InStarCount);

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

protected:
    UFUNCTION(BlueprintCallable, Category = "SaveData")
    void InitializeUnitMap();

public:
    FUnitSaveData GetUnitSaveData(int32 UnitID);

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

    //소지 재화
    UPROPERTY(BlueprintReadWrite, Category = "Player Info")
    int32 CurrentPlayerGold;

    UPROPERTY(BlueprintReadWrite, Category = "Player Info")
    int32 CurrentPlayerGem;

    UPROPERTY(BlueprintReadWrite, Category = "Player Info")
    int32 CurrentPlayerUnlock;

    //스테이지
    UPROPERTY(BlueprintReadWrite, Category = "Player Info")
    int32 CurrentStageIndex = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SaveData")
    TMap<int32, FUnitSaveData> UnitLevelMap;

    FOnGoodsChanged OnGoodsChanged;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
    TObjectPtr<UDataTable> UnitDataTable;

private:
    UPROPERTY()
    TObjectPtr<UPGSaveGame> CachedSaveData;

    FString SaveSlotName = TEXT("PG_SaveSlot_01");
};
