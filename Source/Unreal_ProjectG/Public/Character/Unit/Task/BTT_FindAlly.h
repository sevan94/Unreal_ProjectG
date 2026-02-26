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
    UPROPERTY(EditAnywhere, Category = "Search")
    float SearchRadius = 1500.0f;
};
