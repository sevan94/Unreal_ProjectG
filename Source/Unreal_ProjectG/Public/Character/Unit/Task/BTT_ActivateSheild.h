// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "GameplayTagContainer.h"
#include "BTT_ActivateSheild.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_PROJECTG_API UBTT_ActivateSheild : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
    UBTT_ActivateSheild();

protected:
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
    virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

    // 비헤이비어 트리가 이 노드를 강제로 취소할 때 호출됨
    virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
    UPROPERTY(EditAnywhere, Category = "GAS")
    FGameplayTag ShieldAbilityTag;

    UPROPERTY(EditAnywhere, Category = "GAS")
    FGameplayTag ShieldStateTag;

    UPROPERTY(EditAnywhere, Category = "Blackboard")
    FBlackboardKeySelector SpecialActionReadyKey;

    // 타겟(적)이 아직 살아있는지 확인하기 위한 키
    UPROPERTY(EditAnywhere, Category = "Blackboard")
    FBlackboardKeySelector EnemyTargetKey;
};
