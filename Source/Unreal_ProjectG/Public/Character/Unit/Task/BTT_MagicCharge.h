// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_MagicCharge.generated.h"

class UNiagaraSystem;
class UNiagaraComponent;
class UAnimMontage;
/**
 * 
 */
UCLASS()
class UNREAL_PROJECTG_API UBTT_MagicCharge : public UBTTaskNode
{
	GENERATED_BODY()
public:
    UBTT_MagicCharge();

protected:
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
    virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

public:
    UPROPERTY(EditAnywhere, Category = "Magic")
    float ChargeTime = 2.0f;

    UPROPERTY(EditAnywhere, Category = "Magic")
    UAnimMontage* ChargeAnim;

    UPROPERTY(EditAnywhere, Category = "Magic")
    UNiagaraSystem* ChargeEffect;

private:
    void OnChargeFinished(UBehaviorTreeComponent* OwnerComp);

    UPROPERTY()
    UNiagaraComponent* SpawnedEffect;

    FTimerHandle ChargeTimerHandle;
};
