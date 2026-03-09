// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Character/Unit/UnitCharacter.h"
#include "PGGameInstance.generated.h"

class UPGSaveGame;
class UUnitUIDataAsset;
class UEquipUIDataAsset;
class AHeroCharacter;

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

    //로비에서 선택한 스테이지 번호(게임 모드에 전달할 목적)
    UPROPERTY(BlueprintReadWrite, Category = "Stage")
    int32 SelectedStageNum = 1;

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
private:
    UPROPERTY()
    TObjectPtr<UPGSaveGame> CachedSaveData;

    FString SaveSlotName = TEXT("PG_SaveSlot_01");
};
