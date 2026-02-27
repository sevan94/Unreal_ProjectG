#include "Character/Unit/Task/BTT_MoveToAlly.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "NavigationSystem.h"

UBTT_MoveToAlly::UBTT_MoveToAlly()
{
    NodeName = "Move To Ally (Positioning)";
    AcceptableRadius = 50.0f;
}

EBTNodeResult::Type UBTT_MoveToAlly::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    AAIController* MyController = OwnerComp.GetAIOwner();

    if (!BlackboardComp || !MyController) return EBTNodeResult::Failed;

    AActor* Ally = Cast<AActor>(BlackboardComp->GetValueAsObject(AllyKey.SelectedKeyName));
    AActor* Target = Cast<AActor>(BlackboardComp->GetValueAsObject(AttackTargetKey.SelectedKeyName));
    float Range = BlackboardComp->GetValueAsFloat(AttackRangeKey.SelectedKeyName);

    if (!Ally || !Target) return EBTNodeResult::Failed;

    FVector Direction = (Ally->GetActorLocation() - Target->GetActorLocation()).GetSafeNormal();
    FVector DesiredPos = Target->GetActorLocation() + (Direction * Range);

    UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
    FNavLocation ProjectedLocation;
    if (NavSys && NavSys->ProjectPointToNavigation(DesiredPos, ProjectedLocation, FVector(500.f)))
    {
        MyController->MoveToLocation(ProjectedLocation.Location, AcceptableRadius);
        return EBTNodeResult::Succeeded;
    }

    return EBTNodeResult::Failed;
}