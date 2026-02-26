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
    // 블랙보드 키 필터 설정 (AActor 기반 객체만 허용)
    BlackboardKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTT_FindAlly, BlackboardKey), AActor::StaticClass());
}

EBTNodeResult::Type UBTT_FindAlly::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* AIController = OwnerComp.GetAIOwner();
    if (!AIController) return EBTNodeResult::Failed;

    AUnitCharacter* SelfUnit = Cast<AUnitCharacter>(AIController->GetPawn());
    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();

    if (!SelfUnit || !BlackboardComp)
    {
        UE_LOG(LogTemp, Error, TEXT("FindAlly: SelfUnit 또는 BlackboardComp가 유효하지 않습니다."));
        return EBTNodeResult::Failed;
    }

    UAbilitySystemComponent* SelfASC = SelfUnit->GetAbilitySystemComponent();
    if (!SelfASC)
    {
        UE_LOG(LogTemp, Error, TEXT("FindAlly: SelfASC가 유효하지 않습니다."));
        return EBTNodeResult::Failed;
    }

    // 1. 내 진영(팀) 확인
    FGameplayTag MySideTag;
    if (SelfASC->HasMatchingGameplayTag(PGGameplayTags::Unit_Side_Ally))
    {
        MySideTag = PGGameplayTags::Unit_Side_Ally;
    }
    else if (SelfASC->HasMatchingGameplayTag(PGGameplayTags::Unit_Side_Foe))
    {
        MySideTag = PGGameplayTags::Unit_Side_Foe;
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("FindAlly: 자기 자신에게 진영(Side) 태그가 없습니다."));
        return EBTNodeResult::Failed;
    }

    UE_LOG(LogTemp, Warning, TEXT("FindAlly: 실행 유닛 [%s] / 진영 태그 [%s]"), *SelfUnit->GetName(), *MySideTag.ToString());

    // 2. 주변 탐색 (Overlap)
    TArray<FOverlapResult> OverlapResults;
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(SelfUnit); // 자기 자신은 탐색에서 제외

    FVector SearchOrigin = SelfUnit->GetActorLocation();

    GetWorld()->OverlapMultiByChannel(
        OverlapResults,
        SearchOrigin,
        FQuat::Identity,
        ECC_Pawn, // 주의: 유닛의 충돌 채널이 Pawn이 맞는지 확인하세요.
        FCollisionShape::MakeSphere(SearchRadius),
        QueryParams
    );

    UE_LOG(LogTemp, Warning, TEXT("FindAlly: 반경 %f 내 Overlap된 액터 수 = %d"), SearchRadius, OverlapResults.Num());

    AUnitCharacter* NearestAlly = nullptr;
    float MinDistSq = MAX_FLT;

    // 3. 탐색된 액터 필터링
    for (const FOverlapResult& Result : OverlapResults)
    {
        AUnitCharacter* OtherUnit = Cast<AUnitCharacter>(Result.GetActor());

        if (!OtherUnit || OtherUnit == SelfUnit) continue;

        UAbilitySystemComponent* OtherASC = OtherUnit->GetAbilitySystemComponent();
        if (!OtherASC) continue;

        UE_LOG(LogTemp, Log, TEXT("검사 중인 액터: %s"), *OtherUnit->GetName());

        // 3-1. 진영 체크
        if (!OtherASC->HasMatchingGameplayTag(MySideTag))
        {
            UE_LOG(LogTemp, Log, TEXT("  -> 탈락: 진영 태그가 다름."));
            continue;
        }

        // 3-2. 역할(병과) 체크
        bool bIsMelee = OtherASC->HasMatchingGameplayTag(PGGameplayTags::Unit_Branch_Melee);
        bool bIsTank = OtherASC->HasMatchingGameplayTag(PGGameplayTags::Unit_Branch_Tank);

        if (!bIsMelee && !bIsTank)
        {
            UE_LOG(LogTemp, Log, TEXT("  -> 탈락: 근거리(Melee) 또는 탱커(Tank) 태그가 없음."));
            continue;
        }

        // 3-3. 거리 갱신
        float DistSq = FVector::DistSquared(SearchOrigin, OtherUnit->GetActorLocation());
        if (DistSq < MinDistSq)
        {
            MinDistSq = DistSq;
            NearestAlly = OtherUnit;
        }
    }

    // 4. 최종 결과 처리
    if (NearestAlly)
    {
        UE_LOG(LogTemp, Warning, TEXT("FindAlly: 최종 아군 선택 성공! -> %s"), *NearestAlly->GetName());
        BlackboardComp->SetValueAsObject(BlackboardKey.SelectedKeyName, NearestAlly);
        return EBTNodeResult::Succeeded;
    }

    UE_LOG(LogTemp, Warning, TEXT("FindAlly: 조건에 맞는 아군을 찾지 못했습니다."));
    BlackboardComp->SetValueAsObject(BlackboardKey.SelectedKeyName, nullptr);
    return EBTNodeResult::Failed;
}