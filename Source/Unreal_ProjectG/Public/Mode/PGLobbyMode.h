// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "DataAssets/UI/UnitUIDataAsset.h"
#include "PGLobbyMode.generated.h"

class AGachaActor;
class AEquipGachaActor;

/**
 * 
 */
UCLASS()
class UNREAL_PROJECTG_API APGLobbyMode : public AGameModeBase
{
	GENERATED_BODY()

public:
    void PlayUnitGacha(UUnitUIDataAsset* InData);
    void PlayEquipGacha();
	
protected:
    virtual void BeginPlay() override;

    UFUNCTION()
    void OnGachaAnimationFinished();
    UFUNCTION()
    void OnChestAnimationFinished();

public:
    // 스폰할 가챠 액터 클래스 (에디터에서 할당)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gacha")
    TSubclassOf<AActor> GachaActorClass;

    // 스폰된 유닛 가챠 액터 인스턴스 저장
    UPROPERTY(BlueprintReadWrite, Category = "Gacha")
    TObjectPtr<AGachaActor> UnitGachaActor;

    // 레벨에 존재하는 장비 가챠 액터 인스턴스 저장
    UPROPERTY(BlueprintReadWrite, Category = "Gacha")
    TObjectPtr<AEquipGachaActor> EquipGachaActor;

    // 가챠 스폰 위치 (에디터에서 수정)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gacha")
    FTransform GachaStartTransform;

    // 시네 카메라
    UPROPERTY(BlueprintReadWrite, Category = "Gacha")
    TObjectPtr<AActor> TargetCamera;
};
