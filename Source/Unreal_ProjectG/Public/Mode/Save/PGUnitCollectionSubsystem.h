// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "PGUnitCollectionSubsystem.generated.h"

class UUnitUIDataAsset;
class UPGSaveGame;

// 도감 해금 시 UI 갱신을 위한 델리게이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUnitUnlocked, UUnitUIDataAsset*, UnlockedUnit);

/**
 * 플레이어의 유닛 수집(보유) 상태를 관리하는 서브시스템
 */
UCLASS()
class UNREAL_PROJECTG_API UPGUnitCollectionSubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    virtual void Deinitialize() override;

    // --- [가챠 및 도감 시스템] ---

    // 뽑기 결과 처리 (중복 판별 -> 재화 지급 or 신규 도감 등록)
    UFUNCTION(BlueprintCallable, Category = "UnitCollection|Gacha")
    void ProcessGachaResult(UUnitUIDataAsset* PickedUnitData);

    // 해당 유닛을 보유하고 있는지 확인
    UFUNCTION(BlueprintPure, Category = "UnitCollection")
    bool IsUnitOwned(UUnitUIDataAsset* UnitData) const;

    // 보유 중인 전체 유닛 목록 반환 (도감 UI용)
    UFUNCTION(BlueprintPure, Category = "UnitCollection")
    TArray<UUnitUIDataAsset*> GetOwnedUnits() const;

    // --- [Save / Load 연동] ---
    UFUNCTION(BlueprintCallable, Category = "UnitCollection|SaveLoad")
    void LoadFromSaveGame(const UPGSaveGame* SaveGame);

    UFUNCTION(BlueprintCallable, Category = "UnitCollection|SaveLoad")
    void SaveToSaveGame(UPGSaveGame* SaveGame) const;

public:
    // 이벤트 디스패처
    UPROPERTY(BlueprintAssignable, Category = "UnitCollection|Events")
    FOnUnitUnlocked OnUnitUnlocked;

private:
    // 보유 중인 유닛 리스트 (도감 데이터)
    UPROPERTY()
    TArray<TObjectPtr<UUnitUIDataAsset>> OwnedUnits;
};