// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "SkillAbilityTask_Presentation.generated.h"

class UCameraComponent;
class UDataAsset_HeroSkillPresentation;
class USpringArmComponent;
/**
 * 
 */
UCLASS()
class UNREAL_PROJECTG_API USkillAbilityTask_Presentation : public UAbilityTask
{
	GENERATED_BODY()

public:
    static USkillAbilityTask_Presentation* Create(UGameplayAbility* OwningAbility, UDataAsset_HeroSkillPresentation* InPresentationData);
	
    virtual void Activate() override;
    virtual void TickTask(float DeltaTime) override;
    virtual void OnDestroy(bool bInOwnerFinished) override;

    void ForceRestorAndEnd(); // 프레젠테이션이 끝나지 않았는데 강제로 끝내야 하는 경우, 예를 들어 스킬이 취소된 경우 등

private:
    UFUNCTION()
    void OnImpactPresentationEvent(FGameplayEventData Payload);

    void StartZoomTo(float InTargetFOV, float InDuration, bool bEndWhenDone = false);
    void ApplyHitStopAndHighlight();
    void EndHitStop();
    void RestoreImmediate();

private:
    UPROPERTY()
    TObjectPtr<UDataAsset_HeroSkillPresentation> PresentationData = nullptr;

    TWeakObjectPtr<UCameraComponent> HeroCamera;
    TWeakObjectPtr<USpringArmComponent> SpringArm;

    float OriginalFOV = 0.0f;

    float OriginalSpringArmLength = 0.0f;

    FVector OriginalSpringArmTargetOffset = FVector::ZeroVector;

    FVector OriginalSpringArmSocketOffset = FVector::ZeroVector;

    FRotator OriginalSpringArmRotation = FRotator::ZeroRotator;

    float ZoomedSpringArmLength = 0.0f;
    FVector ZoomedSpringArmTargetOffset = FVector::ZeroVector;
    FVector ZoomedSpringArmSocketOffset = FVector::ZeroVector;
    FRotator ZoomedSpringArmRotation = FRotator::ZeroRotator;

    float LerpElapsed = 0.0f;
    float LerpDuration = 0.0f;
    float LerpStartFOV = 0.0f;
    float LerpTargetFOV = 0.0f;
    float LerpStartSpringArmLength = 0.0f;
    float LerpTargetSpringArmLength = 0.0f;
    FVector LerpStartSpringArmTargetOffset = FVector::ZeroVector;
    FVector LerpTargetSpringArmTargetOffset = FVector::ZeroVector;
    FVector LerpStartSpringArmSocketOffset = FVector::ZeroVector;
    FVector LerpTargetSpringArmSocketOffset = FVector::ZeroVector;
    FRotator LerpStartSpringArmRotation = FRotator::ZeroRotator;
    FRotator LerpTargetSpringArmRotation = FRotator::ZeroRotator;

    FTimerHandle HitStopTimerHandle;
    float CachedBaseGlobalTimeDilation = 1.0f;

    bool bLerpActive = false;
    bool bEndTaskWhenLerpDone = false;
    bool bImpactHandled = false;
    bool bHitStopActive = false;
};
