// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTT_SetPosition.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_PROJECTG_API UBTT_SetPosition : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
    UBTT_SetPosition();

protected:
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
    class AUnitCharacter* FindAlly(class AUnitCharacter* SelfUnit, FVector TargetLocation);

public:
    UPROPERTY(EditAnywhere, Category = "Blackboard")
    FBlackboardKeySelector LocationKey;
private:
    UPROPERTY(EditAnywhere, Category = "Blackboard")
    FBlackboardKeySelector AttackTargetKey;

    UPROPERTY(EditAnywhere, Category = "Blackboard")
    FBlackboardKeySelector AttackRangeKey;
};
