// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTT_SearchAttackArea.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_PROJECTG_API UBTT_SearchAttackArea : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
    UBTT_SearchAttackArea();

protected:
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

public:
    UPROPERTY(EditAnywhere, Category = "Search")
    float SearchRadius = 1500.0f;

    UPROPERTY(EditAnywhere, Category = "Search")
    float ExplosionRadius = 300.0f;

    UPROPERTY(EditAnywhere, Category = "Search")
    FGameplayTag EnemyTag;
};
