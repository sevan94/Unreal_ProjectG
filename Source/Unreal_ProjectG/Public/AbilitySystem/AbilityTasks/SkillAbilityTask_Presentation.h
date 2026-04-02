// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "SkillAbilityTask_Presentation.generated.h"

class UDataAsset_HeroSkillPresentation;
/**
 * 
 */
UCLASS()
class UNREAL_PROJECTG_API USkillAbilityTask_Presentation : public UAbilityTask
{
	GENERATED_BODY()

public:
    static USkillAbilityTask_Presentation* Create(UGameplayAbility* OwningAbility, UDataAsset_HeroSkillPresentation* PresentationData);
	
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
    TObjectPtr<UDataAsset_HeroSkillPresentation> PresentationData = nullptr;

    TWeakObjectPtr<APlayerCameraManager> CameraManager;

    float OriginalFOVOffset = 12.0f;

    float OriginalSpringArmLengthOffset = 0.0f;

    FVector OriginalSpringArmTargetOffset = FVector::ZeroVector;

    FVector OriginalSpringArmSocketOffset = FVector::ZeroVector;

    FRotator OriginalSpringArmRotationOffset = FRotator::ZeroRotator;
};
