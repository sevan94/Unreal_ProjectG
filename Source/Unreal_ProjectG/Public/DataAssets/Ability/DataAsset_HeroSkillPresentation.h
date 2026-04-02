// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DataAsset_HeroSkillPresentation.generated.h"

class UNiagaraSystem;

/**
 * Hero skill presentation configuration for zoom, hit-stop, and impact visuals.
 */
UCLASS()
class UNREAL_PROJECTG_API UDataAsset_HeroSkillPresentation : public UDataAsset
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Presentation|Camera", meta = (DisplayName = "줌 FOV 오프셋"))
    float ZoomFOVOffset = 12.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Presentation|Camera", meta = (DisplayName = "줌 인 시간", ClampMin = "0.0"))
    float ZoomInDuration = 0.12f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Presentation|Camera", meta = (DisplayName = "줌 아웃 시간", ClampMin = "0.0"))
    float ZoomOutDuration = 0.18f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Presentation|SpringArm", meta = (DisplayName = "스프링 암 길이 오프셋"))
    float SpringArmLengthOffset = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Presentation|SpringArm", meta = (DisplayName = "스프링 암 타겟 오프셋"))
    FVector SpringArmTargetOffset = FVector::ZeroVector;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Presentation|SpringArm", meta = (DisplayName = "스프링암 소켓 오프셋"))
    FVector SpringArmSocketOffset = FVector::ZeroVector;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Presentation|SpringArm", meta = (DisplayName = "스프링암 회전 오프셋"))
    FRotator SpringArmRotationOffset = FRotator::ZeroRotator;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Presentation|HitStop", meta = (DisplayName = "스톱 딜레이션", ClampMin = "0.01", ClampMax = "1.0"))
    float HitStopDilation = 0.08f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Presentation|HitStop", meta = (DisplayName = "스톱 시간", ClampMin = "0.0"))
    float HitStopDuration = 0.08f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Presentation|Impact", meta = (DisplayName = "임팩트 나이아가라"))
    TObjectPtr<UNiagaraSystem> ImpactNiagara = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Presentation|Impact", meta = (DisplayName = "임팩트 소켓 이름"))
    FName ImpactNiagaraSocketName = NAME_None;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Presentation|CameraShake", meta = (DisplayName = "카메라 쉐이크"))
    TSubclassOf<UCameraShakeBase> CameraShake = nullptr;
};
