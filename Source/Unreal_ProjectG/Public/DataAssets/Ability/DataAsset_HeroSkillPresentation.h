// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "DataAsset_HeroSkillPresentation.generated.h"

class UNiagaraSystem;
class UCameraShakeBase;

/**
 * Hero skill presentation configuration for zoom, hit-stop, and impact visuals.
 */
UCLASS()
class UNREAL_PROJECTG_API UDataAsset_HeroSkillPresentation : public UDataAsset
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Presentation|Camera", meta = (DisplayName = "카메라 타깃 FOV", ClampMin = "5.0", ClampMax = "170.0"))
    float ZoomFOVTarget = 58.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Presentation|Camera", meta = (DisplayName = "줌 인 시간", ClampMin = "0.0"))
    float ZoomInDuration = 0.12f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Presentation|Camera", meta = (DisplayName = "줌 아웃 시간", ClampMin = "0.0"))
    float ZoomOutDuration = 0.18f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Presentation|SpringArm", meta = (DisplayName = "목표 스프링암 길이", ClampMin = "0.0"))
    float SpringArmLengthTarget = 350.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Presentation|SpringArm", meta = (DisplayName = "목표 SpringArm Location"))
    FVector SpringArmTargetLocation = FVector::ZeroVector;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Presentation|SpringArm", meta = (DisplayName = "목표 SpringArm Socket Location"))
    FVector SpringArmSocketLocation = FVector::ZeroVector;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Presentation|SpringArm", meta = (DisplayName = "목표 SpringArm Rotation"))
    FRotator SpringArmRotationTarget = FRotator::ZeroRotator;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Presentation|HitStop", meta = (DisplayName = "Dilation", ClampMin = "0.01", ClampMax = "1.0"))
    float HitStopDilation = 0.08f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Presentation|HitStop", meta = (DisplayName = "Dilation 기간", ClampMin = "0.0"))
    float HitStopDuration = 0.08f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Presentation|Impact", meta = (DisplayName = "연출용 나이아가라"))
    TObjectPtr<UNiagaraSystem> ImpactNiagara = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Presentation|Impact", meta = (DisplayName = "연출용 나이아가라 소켓"))
    FName ImpactNiagaraSocketName = NAME_None;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Presentation|Impact", meta = (DisplayName = "Impact Event Tag", Categories = "Event.Trigger"))
    FGameplayTag ImpactEventTag;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Presentation|CameraShake", meta = (DisplayName = "카메라 셰이크"))
    TSubclassOf<UCameraShakeBase> CameraShake = nullptr;
};
