// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Types/PGEnumTypes.h"
#include "PGBaseGameMode.generated.h"

<<<<<<< Updated upstream
class ABaseStructure;

/**
 * 
 */
USTRUCT(BlueprintType)
struct FBattleResultData
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Result")
    bool bIsVictory = false;

    UPROPERTY(BlueprintReadOnly, Category = "Result")
    int32 StarCount = 0;

    UPROPERTY(BlueprintReadOnly, Category = "Result")
    float TotalPlayTime = 0.0f;

    UPROPERTY(BlueprintReadOnly, Category = "Result")
    float RemainingHealthPercent = 0.0f;

    UPROPERTY(BlueprintReadOnly, Category = "Result")
    float TotalSpentCost = 0;
=======
//각 스테이지마다 달라질 설정들을 묶어둔 구조체
USTRUCT(BlueprintType)
struct FPGStageInfo
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stage Data")
    float ClearTimeLimit_3Stars = 60.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stage Data")
    float ClearTimeLimit_2Stars = 180.0f;

    // 나중에 여기에 '적군 스폰 속도', '적군 본진 MaxHP' 같은 것도 추가 가능
>>>>>>> Stashed changes
};

UCLASS()
class UNREAL_PROJECTG_API APGBaseGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    APGBaseGameMode();

    //현재 플레이 중인 스테이지 번호
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stage")
    int32 CurrentStageNum;

    //언리얼 에디터(블루프린트)에서 1~5 스테이지의 정보를 표처럼 세팅할 수 있는 맵
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stage")
    TMap<int32, FPGStageInfo> StageDataMap;

    //// --- [1] 인구수 제한  ---
    //UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UnitCount")
    //int32 MAX_ALLY_COUNT = 15; //아군

    //UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UnitCount")
    //int32 MAX_ENEMY_COUNT = 15; //적군

    ////현재 유닛 수 확인
    //UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GameRule")
    //int32 CurrentAllyCount = 0;

    //UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GameRule")
    //int32 CurrentEnemyCount = 0;

   /* bool CanSpawnUnit(ETeamType Team);
    void RegisterUnit(ETeamType Team);
    void UnregisterUnit(ETeamType Team);*/

    // 현재 플레이 시간(초)을 반환하는 함수 (UI 표시용)
    UFUNCTION(BlueprintPure, Category = "GameRule|Time")
    float GetCurrentPlayTime() const;

    //기지 파괴 시 호출될 게임오버 함수
    UFUNCTION()
    void OnGameOver(ETeamType DefeatedTeam);

protected:
    virtual void BeginPlay() override;

    //  UI 출력 이벤트
    UFUNCTION(BlueprintImplementableEvent, Category = "GameRule")
    void BP_ShowResultUI(const FBattleResultData& ResultData);

public:
    UPROPERTY(BlueprintReadWrite, Category = "Battle")
    int32 SpentCost = 0;

protected:

    // ---  클리어 등급 설정 (시간 제한) ---
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameRule|Grade")
    float ClearTimeLimit_3Stars = 60.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameRule|Grade")
    float ClearTimeLimit_2Stars = 180.0f;

    // 상태 및 시간 저장용 변수
    bool bIsGameOver = false;
    float GameStartTime = 0.0f;

    UPROPERTY(BlueprintReadWrite, Category = "Battle")
    float TotalSpentCost = 0;

private:
    TObjectPtr<ABaseStructure> AllyBase;
};
