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
    // 블랙보드 키 필터링 (Object 타입만 선택 가능하도록)
    BlackboardKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTT_FindAlly, BlackboardKey), AActor::StaticClass());
}

EBTNodeResult::Type UBTT_FindAlly::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

    AUnitCharacter* SelfUnit = Cast<AUnitCharacter>(OwnerComp.GetAIOwner()->GetPawn());
    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();

    if (!SelfUnit || !BlackboardComp) return EBTNodeResult::Failed;

    UAbilitySystemComponent* SelfASC = SelfUnit->GetAbilitySystemComponent();
    if (!SelfASC) return EBTNodeResult::Failed;

    // 자신의 진영 확인
    FGameplayTag MySideTag = SelfASC->HasMatchingGameplayTag(PGGameplayTags::Unit_Side_Ally)
        ? PGGameplayTags::Unit_Side_Ally : PGGameplayTags::Unit_Side_Foe;

    TArray<FOverlapResult> OverlapResults;
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(SelfUnit);

    GetWorld()->OverlapMultiByChannel(
        OverlapResults,
        SelfUnit->GetActorLocation(),
        FQuat::Identity,
        ECC_Pawn,
        FCollisionShape::MakeSphere(SearchRadius),
        QueryParams
    );

    UE_LOG(LogTemp, Warning, TEXT("FindAlly: Searching for Side Tag : %s"), *MySideTag.ToString());
    UE_LOG(LogTemp, Warning, TEXT("Overlap Count: %d"), OverlapResults.Num());

    AUnitCharacter* NearestAlly = nullptr;
    float MinDistSq = MAX_FLT;

    for (const FOverlapResult& Result : OverlapResults)
    {
        AUnitCharacter* OtherUnit = Cast<AUnitCharacter>(Result.GetActor());
        if (!OtherUnit) continue;

        UAbilitySystemComponent* OtherASC = OtherUnit->GetAbilitySystemComponent();
        if (OtherASC && OtherASC->HasMatchingGameplayTag(MySideTag))
        {
            //사망 확인 해야함
            //bool bIsLive = !OtherASC->HasMatchingGameplayTag(PGGameplayTags::State_Dead);
            bool bIsMelee = OtherASC->HasMatchingGameplayTag(PGGameplayTags::Unit_Branch_Melee);
            bool bIsTank = OtherASC->HasMatchingGameplayTag(PGGameplayTags::Unit_Branch_Tank);

            //if (bIsLive && (bIsMelee || bIsTank))
            //{
            //    float DistSq = FVector::DistSquared(SelfUnit->GetActorLocation(), OtherUnit->GetActorLocation());
            //    if (DistSq < MinDistSq)
            //    {
            //        MinDistSq = DistSq;
            //        NearestAlly = OtherUnit;
            //    }
            //}
        }
    }

    if (NearestAlly)
    {
        BlackboardComp->SetValueAsObject(BlackboardKey.SelectedKeyName, NearestAlly);
        return EBTNodeResult::Succeeded;
    }

    // 아군을 못 찾았으면 블랙보드 비우고 실패 반환
    BlackboardComp->SetValueAsObject(BlackboardKey.SelectedKeyName, nullptr);
    return EBTNodeResult::Failed;
}