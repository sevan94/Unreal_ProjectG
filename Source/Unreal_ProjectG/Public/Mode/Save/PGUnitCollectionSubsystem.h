// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "PGUnitCollectionSubsystem.generated.h"

class UUnitUIDataAsset;
class UPGSaveGame;

// 도감 해금 시 UI 갱신용 델리게이트
// DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGachaUnitUnlocked, UUnitUIDataAsset*, UnlockedUnit);

UCLASS()
class UNREAL_PROJECTG_API UPGUnitCollectionSubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    // ==========================================================
    // --- [1. 뽑기(Gacha) 시스템] ---
    // ==========================================================

    // 단일 뽑기: 기획자가 설정한 유닛 풀(배열)에서 1개를 무작위로 뽑아 반환
    UFUNCTION(BlueprintCallable, Category = "UnitCollection|Gacha")
    UUnitUIDataAsset* RollSingleGacha();

    // 연속 뽑기(10연뽑 등): 지정한 횟수만큼 뽑아 배열로 반환
    UFUNCTION(BlueprintCallable, Category = "UnitCollection|Gacha")
    TArray<UUnitUIDataAsset*> RollMultiGacha(const TArray<UUnitUIDataAsset*>& GachaPool, int32 PullCount = 10);

    // 뽑기 결과 처리 (중복 판별 -> 재화 지급 or 신규 도감 등록)
    UFUNCTION(BlueprintCallable, Category = "UnitCollection|Gacha")
    void ProcessGachaResult(UUnitUIDataAsset* PickedUnitData);


    // ==========================================================
    // --- [2. 도감(UnitLevelMap) 상태 조회 시스템] ---
    // ==========================================================

    // 특정 유닛(ID 기반)을 보유하고 있는지 확인
    UFUNCTION(BlueprintPure, Category = "UnitCollection")
    bool IsUnitOwned(int32 UnitID) const;

    // 특정 유닛의 현재 레벨 확인
    UFUNCTION(BlueprintPure, Category = "UnitCollection")
    int32 GetUnitLevel(int32 UnitID) const;

public:
    // 이벤트 디스패처
    //UPROPERTY(BlueprintAssignable, Category = "UnitCollection|Events")
    //FOnGachaUnitUnlocked OnGachaUnitUnlocked;

    // 기존에 있던 OwnedUnits 배열과 Save/Load 함수는 
    // GameInstance의 UnitLevelMap이 전담하게 되었으므로 삭제
};