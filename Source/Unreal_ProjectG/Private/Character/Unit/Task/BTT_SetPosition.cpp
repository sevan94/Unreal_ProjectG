// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Unit/Task/BTT_SetPosition.h"
#include "Character/Unit/AI/UnitDetourCrowdAIController.h"
#include "Character/Unit/UnitCharacter.h" 
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"
#include "Kismet/KismetSystemLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "PGGameplayTags.h" 
#include "Engine/OverlapResult.h"

UBTT_SetPosition::UBTT_SetPosition()
{
    NodeName = "SetPosition";
}

EBTNodeResult::Type UBTT_SetPosition::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AUnitDetourCrowdAIController* AIController = Cast<AUnitDetourCrowdAIController>(OwnerComp.GetAIOwner());
    if (!AIController) return EBTNodeResult::Failed;

    AUnitCharacter* SelfUnit = Cast<AUnitCharacter>(AIController->GetPawn());
    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    if (!SelfUnit || !BlackboardComp) return EBTNodeResult::Failed;

    AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject(AttackTargetKey.SelectedKeyName));
    float AttackRange = BlackboardComp->GetValueAsFloat(AttackRangeKey.SelectedKeyName);

    if (!TargetActor || AttackRange <= 0.0f) return EBTNodeResult::Failed;

    FVector TargetLoc = TargetActor->GetActorLocation();
    FVector SelfLoc = SelfUnit->GetActorLocation();
    FVector IdealDirection;

    AUnitCharacter* NearestAlly = FindAlly(SelfUnit, TargetLoc);

    if (NearestAlly)
    {
        IdealDirection = (NearestAlly->GetActorLocation() - TargetLoc).GetSafeNormal();
    }
    else
    {
        IdealDirection = (SelfLoc - TargetLoc).GetSafeNormal();
    }

    FVector DesiredLocation = TargetLoc + (IdealDirection * AttackRange);

    UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
    FNavLocation ProjectedLocation;

    if (NavSys && NavSys->ProjectPointToNavigation(DesiredLocation, ProjectedLocation, FVector(500.0f, 500.0f, 500.0f)))
    {
        AIController->MoveToLocation(ProjectedLocation.Location, 50.0f, false, true, true);
        return EBTNodeResult::Succeeded;
    }

    return EBTNodeResult::Failed;
}

AUnitCharacter* UBTT_SetPosition::FindAlly(AUnitCharacter* SelfUnit, FVector TargetLocation)
{
    if (!SelfUnit || !SelfUnit->GetWorld()) return nullptr;

    AUnitCharacter* NearestAlly = nullptr;
    float MinDistanceSquared = MAX_FLT;
    float SearchRadius = 1500.0f; 

    FVector SearchOrigin = SelfUnit->GetActorLocation();
    UWorld* World = SelfUnit->GetWorld();

    TArray<FOverlapResult> OverlapResults;
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(SelfUnit);

    World->OverlapMultiByChannel(
        OverlapResults,
        SearchOrigin,
        FQuat::Identity,
        ECC_Pawn,
        FCollisionShape::MakeSphere(SearchRadius),
        QueryParams
    );

    UAbilitySystemComponent* SelfASC = SelfUnit->GetAbilitySystemComponent(); 
    if (!SelfASC) return nullptr;

    bool bIsSelfAlly = SelfASC->HasMatchingGameplayTag(PGGameplayTags::Unit_Side_Ally);
    FGameplayTag MySideTag = bIsSelfAlly ? PGGameplayTags::Unit_Side_Ally : PGGameplayTags::Unit_Side_Foe;

    for (const FOverlapResult& Result : OverlapResults)
    {
        AUnitCharacter* OtherUnit = Cast<AUnitCharacter>(Result.GetActor());

        // 죽은 유닛은 방패막이로 쓸 수 없으므로 생존 여부 체크해야함
        if (OtherUnit)
        {
            UAbilitySystemComponent* OtherASC = OtherUnit->GetAbilitySystemComponent();
            if (OtherASC)
            {
                if (OtherASC->HasMatchingGameplayTag(MySideTag))
                {
                    bool bIsMelee = OtherASC->HasMatchingGameplayTag(PGGameplayTags::Unit_Branch_Melee);
                    bool bIsTank = OtherASC->HasMatchingGameplayTag(PGGameplayTags::Unit_Branch_Tank);

                    if (bIsMelee || bIsTank)
                    {
                        float DistSq = FVector::DistSquared(SearchOrigin, OtherUnit->GetActorLocation());
                        if (DistSq < MinDistanceSquared)
                        {
                            MinDistanceSquared = DistSq;
                            NearestAlly = OtherUnit;
                        }
                    }
                }
            }
        }
    }

    return NearestAlly;
}
