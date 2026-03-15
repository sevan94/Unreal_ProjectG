// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTT_FindAlly.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_PROJECTG_API UBTT_FindAlly : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
    UBTT_FindAlly();

    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:

    UPROPERTY(EditAnywhere, Category = "Blackboard")
    FBlackboardKeySelector bIsAllyInFrontKey;

    UPROPERTY(EditAnywhere, Category = "Blackboard")
    FBlackboardKeySelector TargetLocationKey;

    UPROPERTY(EditAnywhere, Category = "Settings")
    float SearchRadius = 1500.0f;

    UPROPERTY(EditAnywhere, Category = "Settings")
    float InFrontCheckDistance = 500.0f;

    UPROPERTY(EditAnywhere, Category = "Settings")
    float DistanceBehindAlly = 200.0f; 
};
