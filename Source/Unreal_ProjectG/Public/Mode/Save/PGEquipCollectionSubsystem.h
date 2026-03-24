// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "PGEquipCollectionSubsystem.generated.h"

class UEquipUIDataAsset;
class UPGSaveGame;

UCLASS()
class UNREAL_PROJECTG_API UPGEquipCollectionSubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    // 단일 뽑기: 기획자가 설정한 유닛 풀(배열)에서 1개를 무작위로 뽑아 반환
    UFUNCTION(BlueprintCallable, Category = "EquipCollection|Gacha")
    UEquipUIDataAsset* RollSingleGacha();

    // 연속 뽑기(10연뽑 등): 지정한 횟수만큼 뽑아 배열로 반환
    //UFUNCTION(BlueprintCallable, Category = "EquipCollection|Gacha")
    //TArray<UEquipUIDataAsset*> RollMultiGacha(const TArray<UEquipUIDataAsset*>& GachaPool, int32 PullCount = 10);

    // 뽑기 결과 처리 (중복 판별 -> 재화 지급 or 신규 도감 등록)
    UFUNCTION(BlueprintCallable, Category = "EquipCollection|Gacha")
    void ProcessGachaResult(UEquipUIDataAsset* PickedEquipData);

    UFUNCTION(BlueprintPure, Category = "EquipCollection")
    bool IsEquipOwned(int32 EquipID) const;
};