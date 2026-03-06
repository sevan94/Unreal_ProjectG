// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Unit/Task/BTT_SearchAttackArea.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameplayTagAssetInterface.h"
#include "Engine/World.h"
#include "Engine/OverlapResult.h"
#include "Engine/World.h"

UBTT_SearchAttackArea::UBTT_SearchAttackArea()
{
    NodeName = "SearchAttackArea";
}

EBTNodeResult::Type UBTT_SearchAttackArea::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* AIController = OwnerComp.GetAIOwner();
    APawn* ControlledPawn = AIController->GetPawn();
    UWorld* World = GetWorld();

    if (!ControlledPawn || !World) return EBTNodeResult::Failed;

    FVector OriginLocation = ControlledPawn->GetActorLocation();
    TArray<AActor*> ValidEnemies;

    TArray<FOverlapResult> OverlapResults;
    FCollisionObjectQueryParams ObjectQueryParams;
    ObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn);

    FCollisionShape CollisionShape;
    CollisionShape.SetSphere(SearchRadius);

    World->OverlapMultiByObjectType(
        OverlapResults,
        OriginLocation,
        FQuat::Identity,
        ObjectQueryParams,
        CollisionShape
    );

    for (const FOverlapResult& Result : OverlapResults)
    {
        AActor* OverlappedActor = Result.GetActor();
        if (OverlappedActor && OverlappedActor != ControlledPawn)
        {
            IGameplayTagAssetInterface* TagInterface = Cast<IGameplayTagAssetInterface>(OverlappedActor);
            if (TagInterface && TagInterface->HasMatchingGameplayTag(EnemyTag))
            {
                ValidEnemies.Add(OverlappedActor);
            }
        }
    }

    if (ValidEnemies.Num() == 0) return EBTNodeResult::Failed; 

    int32 MaxHitCount = 0;
    FVector BestTargetLocation = OriginLocation;

    for (AActor* EnemyA : ValidEnemies)
    {
        int32 CurrentHitCount = 0;
        FVector LocationA = EnemyA->GetActorLocation();

        for (AActor* EnemyB : ValidEnemies)
        {
            float DistanceSq = FVector::DistSquared(LocationA, EnemyB->GetActorLocation());
            if (DistanceSq <= (ExplosionRadius * ExplosionRadius))
            {
                CurrentHitCount++;
            }
        }

        if (CurrentHitCount > MaxHitCount)
        {
            MaxHitCount = CurrentHitCount;
            BestTargetLocation = LocationA;
        }
    }

    OwnerComp.GetBlackboardComponent()->SetValueAsVector(BlackboardKey.SelectedKeyName, BestTargetLocation);

    return EBTNodeResult::Succeeded;
}
