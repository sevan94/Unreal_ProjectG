#include "Character/Unit/Task/BTT_FindAlly.h"
#include "Character/Unit/AI/UnitDetourCrowdAIController.h"
#include "Character/Unit/UnitCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AbilitySystemComponent.h"
#include "PGGameplayTags.h"
#include "Engine/OverlapResult.h"

UBTT_FindAlly::UBTT_FindAlly()
{
    NodeName = "FindAlly";

    BlackboardKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTT_FindAlly, BlackboardKey), AActor::StaticClass());
    bIsAllyInFrontKey.AddBoolFilter(this, GET_MEMBER_NAME_CHECKED(UBTT_FindAlly, bIsAllyInFrontKey));
    TargetLocationKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTT_FindAlly, TargetLocationKey));
    TargetActorKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTT_FindAlly, TargetActorKey), AActor::StaticClass());
}

EBTNodeResult::Type UBTT_FindAlly::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* AIController = OwnerComp.GetAIOwner();
    AUnitCharacter* SelfUnit = (AIController) ? Cast<AUnitCharacter>(AIController->GetPawn()) : nullptr;
    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();

    if (!SelfUnit || !BlackboardComp) return EBTNodeResult::Failed;

    // 1. 진영 태그 결정
    FGameplayTag MySideTag;
    if (SelfUnit->HasMatchingGameplayTag(PGGameplayTags::Unit_Side_Ally)) MySideTag = PGGameplayTags::Unit_Side_Ally;
    else if (SelfUnit->HasMatchingGameplayTag(PGGameplayTags::Unit_Side_Foe)) MySideTag = PGGameplayTags::Unit_Side_Foe;
    else return EBTNodeResult::Failed;

    // 2. 주변 탐색
    TArray<FOverlapResult> OverlapResults;
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(SelfUnit);

    FVector SelfLoc = SelfUnit->GetActorLocation();
    SearchRadius = SelfUnit->GetDetectRangeKey();
    GetWorld()->OverlapMultiByChannel(OverlapResults, SelfLoc, FQuat::Identity, ECC_Pawn, FCollisionShape::MakeSphere(SearchRadius), QueryParams);

    AUnitCharacter* NearestAlly = nullptr;
    float MinDistSq = MAX_FLT;

    for (const FOverlapResult& Result : OverlapResults)
    {
        AUnitCharacter* OtherUnit = Cast<AUnitCharacter>(Result.GetActor());
        if (!OtherUnit || OtherUnit == SelfUnit) continue;

        if (OtherUnit->HasMatchingGameplayTag(MySideTag))
        {
            if (OtherUnit->HasMatchingGameplayTag(PGGameplayTags::Unit_Branch_Melee) ||
                OtherUnit->HasMatchingGameplayTag(PGGameplayTags::Unit_Branch_Tank))
            {
                float DistSq = FVector::DistSquared(SelfLoc, OtherUnit->GetActorLocation());
                if (DistSq < MinDistSq)
                {
                    MinDistSq = DistSq;
                    NearestAlly = OtherUnit;
                }
            }
        }
    }

    if (NearestAlly)
    {
        BlackboardComp->SetValueAsObject(BlackboardKey.SelectedKeyName, NearestAlly);

        FVector SelfLoc2D = FVector(SelfLoc.X, SelfLoc.Y, 0.0f);
        FVector AllyLoc2D = FVector(NearestAlly->GetActorLocation().X, NearestAlly->GetActorLocation().Y, 0.0f);

        AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject(TargetActorKey.SelectedKeyName));
        FVector ForwardDirection;

        if (TargetActor)
        {
            FVector TargetLoc2D = FVector(TargetActor->GetActorLocation().X, TargetActor->GetActorLocation().Y, 0.0f);
            ForwardDirection = (TargetLoc2D - SelfLoc2D).GetSafeNormal();
        }
        else
        {
            ForwardDirection = FVector(SelfUnit->GetActorForwardVector().X, SelfUnit->GetActorForwardVector().Y, 0.0f).GetSafeNormal();
        }

        FVector DirToAlly = (AllyLoc2D - SelfLoc2D).GetSafeNormal();
        float DotProduct = FVector::DotProduct(ForwardDirection, DirToAlly);
        float DistanceToAlly = FVector::Distance(SelfLoc2D, AllyLoc2D);

        bool bIsInFront = (DotProduct > 0.5f) && (DistanceToAlly <= InFrontCheckDistance);
        BlackboardComp->SetValueAsBool(bIsAllyInFrontKey.SelectedKeyName, bIsInFront);

        FVector TargetPos = NearestAlly->GetActorLocation() - (ForwardDirection * DistanceBehindAlly);
        TargetPos.Z = NearestAlly->GetActorLocation().Z;

        BlackboardComp->SetValueAsVector(TargetLocationKey.SelectedKeyName, TargetPos);

        return EBTNodeResult::Succeeded;
    }

    BlackboardComp->SetValueAsObject(BlackboardKey.SelectedKeyName, nullptr);
    BlackboardComp->SetValueAsBool(bIsAllyInFrontKey.SelectedKeyName, false);
    return EBTNodeResult::Failed;
}