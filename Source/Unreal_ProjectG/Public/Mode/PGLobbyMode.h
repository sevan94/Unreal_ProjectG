// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PGLobbyMode.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_PROJECTG_API APGLobbyMode : public AGameModeBase
{
	GENERATED_BODY()
	
protected:
    virtual void BeginPlay() override;

public:
    // 스폰할 가챠 액터 클래스
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gacha")
    TSubclassOf<AActor> GachaActorClass;

    // 스폰된 가챠 액터 인스턴스 저장
    UPROPERTY(BlueprintReadWrite, Category = "Gacha")
    TObjectPtr<AActor> GachaActor;

    // 가챠 스폰 위치 (트랜스폼)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gacha")
    FTransform GachaStartTransform;

    // 시네 카메라
    UPROPERTY(BlueprintReadWrite, Category = "Gacha")
    TObjectPtr<AActor> TargetCamera;
};
