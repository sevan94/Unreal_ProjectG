// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Unit/Task/BTT_ActivateSheild.h"
#include "AIController.h"
#include "Character/Unit/UnitCharacter.h"
#include "AbilitySystemComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTT_ActivateSheild::UBTT_ActivateSheild()
{
    NodeName = "Activate Sheild";
    bNotifyTick = true;
}

EBTNodeResult::Type UBTT_ActivateSheild::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* AIController = OwnerComp.GetAIOwner();
    if (!AIController)
    {
        return EBTNodeResult::Failed;
    }

    AUnitCharacter* Unit = Cast<AUnitCharacter>(AIController->GetPawn());
    UAbilitySystemComponent* ASC = Unit ? Unit->GetAbilitySystemComponent() : nullptr;
    if (!ASC)
    {
        return EBTNodeResult::Failed;
    }

    bool bSuccess = ASC->TryActivateAbilitiesByTag(FGameplayTagContainer(ShieldAbilityTag));
    if (!bSuccess)
    {
        return EBTNodeResult::Failed;
    }

    return EBTNodeResult::InProgress;
}

void UBTT_ActivateSheild::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    AAIController* AIController = OwnerComp.GetAIOwner();
    UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();
    if (!AIController || !BBComp)
    {
        return;
    }

    AUnitCharacter* Unit = Cast<AUnitCharacter>(AIController->GetPawn());
    UAbilitySystemComponent* ASC = Unit ? Unit->GetAbilitySystemComponent() : nullptr;
    if (!ASC)
    {
        return;
    }

    AActor* TargetActor = Cast<AActor>(BBComp->GetValueAsObject(EnemyTargetKey.SelectedKeyName));
    bool bNoEnemy = (!TargetActor || !TargetActor->IsValidLowLevel());
    bool bShieldBroken = !ASC->HasMatchingGameplayTag(ShieldStateTag);

    if (bNoEnemy || bShieldBroken)
    {
        if (bShieldBroken)
        {
            BBComp->SetValueAsBool(SpecialActionReadyKey.SelectedKeyName, false);
        }

        FGameplayTagContainer CancelTags(ShieldAbilityTag);
        ASC->CancelAbilities(&CancelTags);

        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
    }
}

EBTNodeResult::Type UBTT_ActivateSheild::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    if (AAIController* AIController = OwnerComp.GetAIOwner())
    {
        if (AUnitCharacter* Unit = Cast<AUnitCharacter>(AIController->GetPawn()))
        {
            if (UAbilitySystemComponent* ASC = Unit->GetAbilitySystemComponent())
            {
                FGameplayTagContainer CancelTags(ShieldAbilityTag);
                ASC->CancelAbilities(&CancelTags);
            }
        }
    }

    return EBTNodeResult::Aborted;
}
