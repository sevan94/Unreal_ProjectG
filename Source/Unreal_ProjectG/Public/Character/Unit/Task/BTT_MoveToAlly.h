// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_MoveTo.h"
#include "BTT_MoveToAlly.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_PROJECTG_API UBTT_MoveToAlly : public UBTTask_MoveTo
{
	GENERATED_BODY()
public:
    UBTT_MoveToAlly();

    // 이동 시작 시 목적지를 재계산하기 위해 ExecuteTask 오버라이드
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
    UPROPERTY(EditAnywhere, Category = "Config")
    FBlackboardKeySelector AllyKey;

    UPROPERTY(EditAnywhere, Category = "Config")
    FBlackboardKeySelector AttackTargetKey;

    UPROPERTY(EditAnywhere, Category = "Config")
    FBlackboardKeySelector AttackRangeKey;
};
