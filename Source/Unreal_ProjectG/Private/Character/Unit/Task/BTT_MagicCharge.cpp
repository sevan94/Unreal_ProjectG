// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Unit/Task/BTT_MagicCharge.h"
#include "GameFramework/Character.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "TimerManager.h"

UBTT_MagicCharge::UBTT_MagicCharge()
{
    NodeName = TEXT("Charge Magic");
    bCreateNodeInstance = true; 
}

EBTNodeResult::Type UBTT_MagicCharge::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* AIController = OwnerComp.GetAIOwner();
    ACharacter* Character = Cast<ACharacter>(AIController->GetPawn());

    if (!Character) return EBTNodeResult::Failed;

    if (ChargeAnim)
    {
        Character->PlayAnimMontage(ChargeAnim);
    }

    if (ChargeEffect)
    {
        SpawnedEffect = UNiagaraFunctionLibrary::SpawnSystemAttached(
            ChargeEffect, Character->GetMesh(), TEXT("Muzzle"),
            FVector::ZeroVector, FRotator::ZeroRotator,
            EAttachLocation::SnapToTarget, true);
    }

    Character->GetWorldTimerManager().SetTimer(
        ChargeTimerHandle,
        FTimerDelegate::CreateUObject(this, &UBTT_MagicCharge::OnChargeFinished, &OwnerComp),
        ChargeTime,
        false);

    return EBTNodeResult::InProgress;
}

EBTNodeResult::Type UBTT_MagicCharge::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* AIController = OwnerComp.GetAIOwner();
    ACharacter* Character = Cast<ACharacter>(AIController->GetPawn());

    // 1. 타이머 취소
    if (Character)
    {
        Character->GetWorldTimerManager().ClearTimer(ChargeTimerHandle);

        // 2. 몽타주 중단
        if (ChargeAnim)
        {
            Character->StopAnimMontage(ChargeAnim);
        }
    }

    // 3. 이펙트 제거
    if (SpawnedEffect)
    {
        SpawnedEffect->DestroyComponent();
        SpawnedEffect = nullptr;
    }

    return EBTNodeResult::Aborted;
}

void UBTT_MagicCharge::OnChargeFinished(UBehaviorTreeComponent* OwnerComp)
{
    FinishLatentTask(*OwnerComp, EBTNodeResult::Succeeded);
}
