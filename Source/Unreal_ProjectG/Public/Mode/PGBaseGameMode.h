// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Types/PGEnumTypes.h"
#include "PGBaseGameMode.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_PROJECTG_API APGBaseGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    APGBaseGameMode();

    // --- [1] 인구수 제한  ---
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UnitCount")
    int32 MAX_ALLY_COUNT = 15; //아군

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UnitCount")
    int32 MAX_ENEMY_COUNT = 15; //적군

    //현재 유닛 수 확인
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GameRule")
    int32 CurrentAllyCount = 0;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GameRule")
    int32 CurrentEnemyCount = 0;

    bool CanSpawnUnit(ETeamType Team);
    void RegisterUnit(ETeamType Team);
    void UnregisterUnit(ETeamType Team);

    // ---  클리어 등급 설정 (시간 제한) ---
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameRule|Grade")
    float ClearTimeLimit_3Stars = 60.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameRule|Grade")
    float ClearTimeLimit_2Stars = 180.0f;

    // 현재 플레이 시간(초)을 반환하는 함수 (UI 표시용)
    UFUNCTION(BlueprintPure, Category = "GameRule|Time")
    float GetCurrentPlayTime() const;

protected:
    virtual void BeginPlay() override;

public:
    //기지 파괴 시 호출될 게임오버 함수
    UFUNCTION()
    void OnGameOver(ETeamType DefeatedTeam);

protected:
    // 상태 및 시간 저장용 변수
    bool bIsGameOver = false;
    float GameStartTime = 0.0f;

    //  UI 출력 이벤트
    UFUNCTION(BlueprintImplementableEvent, Category = "GameRule")
    void BP_ShowResultUI(bool bIsVictory, int32 StarCount);
};
