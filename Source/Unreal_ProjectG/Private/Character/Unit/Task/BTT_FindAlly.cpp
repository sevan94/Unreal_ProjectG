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

        FVector DirToAlly = (NearestAlly->GetActorLocation() - SelfLoc).GetSafeNormal();
        float DotProduct = FVector::DotProduct(SelfUnit->GetActorForwardVector(), DirToAlly);
        float DistanceToAlly = FVector::Dist(SelfLoc, NearestAlly->GetActorLocation());

        bool bIsInFront = (DotProduct > 0.0f) && (DistanceToAlly <= InFrontCheckDistance);
        BlackboardComp->SetValueAsBool(bIsAllyInFrontKey.SelectedKeyName, bIsInFront);

        FVector TargetPos = NearestAlly->GetActorLocation() - (NearestAlly->GetActorForwardVector() * DistanceBehindAlly);
        BlackboardComp->SetValueAsVector(TargetLocationKey.SelectedKeyName, TargetPos);

        return EBTNodeResult::Succeeded;
    }

    BlackboardComp->SetValueAsObject(BlackboardKey.SelectedKeyName, nullptr);
    BlackboardComp->SetValueAsBool(bIsAllyInFrontKey.SelectedKeyName, false);
    return EBTNodeResult::Failed;
}