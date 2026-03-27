// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Mode/Save/PGGameInstance.h"
#include "PGSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_PROJECTG_API UPGSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
    UPGSaveGame();

    UPROPERTY(VisibleAnywhere, Category = "Basic")
    FString PlayerName;

    UPROPERTY(VisibleAnywhere, Category = "Basic")
    int32 PlayerLevel;

    //재화 관련
    UPROPERTY(VisibleAnywhere, Category = "Basic")
    int32 PlayerGold;

    UPROPERTY(VisibleAnywhere, Category = "Basic")
    int32 PlayerGem;

    UPROPERTY(VisibleAnywhere, Category = "Basic")
    int32 PlayerUnlock;

    // 장착 중인 무기 데이터 에셋의 경로 (SoftObjectPath)
    UPROPERTY(VisibleAnywhere, Category = "Equipment")
    FSoftObjectPath EquippedWeaponPath;

    UPROPERTY(VisibleAnywhere, Category = "Equipment")
    FSoftObjectPath EquippedArmorPath;

    UPROPERTY(VisibleAnywhere, Category = "Equipment")
    FSoftObjectPath EquippedAccessoryPath;

    // 장착 중인 유닛(최대 5개) 데이터 에셋의 경로들
    UPROPERTY(VisibleAnywhere, Category = "Equipment")
    TArray<FSoftObjectPath> EquippedUnitPaths;

    // 유닛 정보 맵
    UPROPERTY()
    TMap<int32, FUnitSaveData> UnitLevelMap;

    // 장비 정보 맵
    UPROPERTY()
    TMap<int32, bool> EquipMap;

    // 스테이지 정보 맵
    UPROPERTY()
    TMap<int32, int32> StageDataMap;

    // --- [환경설정: 사운드] ---
    UPROPERTY(VisibleAnywhere, Category = "Settings|Sound")
    float MasterVolume = 1.0f; // 0.0 ~ 1.0

    UPROPERTY(VisibleAnywhere, Category = "Settings|Sound")
    float BGMVolume = 1.0f;

    UPROPERTY(VisibleAnywhere, Category = "Settings|Sound")
    float SFXVolume = 1.0f;
};
