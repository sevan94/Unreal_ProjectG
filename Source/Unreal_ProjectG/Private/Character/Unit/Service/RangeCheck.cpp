// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Unit/Service/RangeCheck.h"
#include "Character/Unit/AI/UnitDetourCrowdAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/Unit/SubSystem/UnitSubsystem.h"
#include "Components/CapsuleComponent.h"
#include "GameplayTagAssetInterface.h"
#include "PGGameplayTags.h"
#include "DrawDebugHelpers.h"

URangeCheck::URangeCheck()
{
    NodeName = "RangeCheck";
    Interval = 0.5f;
    RandomDeviation = 0.1f;
}

void URangeCheck::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    AUnitDetourCrowdAIController* AIC = Cast<AUnitDetourCrowdAIController>(OwnerComp.GetAIOwner());

    APawn* ControllPawn = OwnerComp.GetAIOwner() ? OwnerComp.GetAIOwner()->GetPawn() : nullptr;
    if (!ControllPawn)
    {
        return;
    }

    UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
    if (!Blackboard)
    {
        return;
    }

    UUnitSubsystem* UnitSubSystem = GetWorld()->GetSubsystem<UUnitSubsystem>();
    if (!UnitSubSystem)
    {
        return;
    }

    float Range = Blackboard->GetValueAsFloat(DetectRangeKey.SelectedKeyName);
    float RangeSq = Range * Range;

    float AttackRange = Blackboard->GetValueAsFloat(AttackRangeKey.SelectedKeyName);
    float AttackRangeSq = AttackRange * AttackRange;

    IGameplayTagAssetInterface* UnitInterface = Cast<IGameplayTagAssetInterface>(ControllPawn);
    if (!UnitInterface)
    {
        return;
    }

    FGameplayTag EnemyTag = UnitInterface->
        HasMatchingGameplayTag(PGGameplayTags::Unit_Side_Ally)
        ? PGGameplayTags::Unit_Side_Foe
        : PGGameplayTags::Unit_Side_Ally;

    const TArray<TWeakObjectPtr<AActor>>& EnemyList = UnitSubSystem->GetUnitsByTeam(EnemyTag);

    AActor* TargetEnemy = nullptr;
    float MinDistSq = RangeSq;
    bool bInDetectRange = false;
    bool bInAttackRange = false;

    FVector MyLocation = ControllPawn->GetActorLocation();
    for (const TWeakObjectPtr<AActor>& EnemyPtr : EnemyList)
    {
        AActor* Enemy = EnemyPtr.Get();
        if (!Enemy) continue;

        float DistSq = FVector::DistSquared(MyLocation, Enemy->GetActorLocation());

        if (DistSq <= MinDistSq)
        {
            MinDistSq = DistSq;
            TargetEnemy = Enemy;
            bInDetectRange = true;
        }
    }

    if (bInDetectRange && TargetEnemy)
    {
        if (MinDistSq <= AttackRangeSq)
        {
            bInAttackRange = true;
        }
    }

    Blackboard->SetValueAsObject(TargetActorKey.SelectedKeyName, TargetEnemy);
    Blackboard->SetValueAsBool(IsInDetectRangeKey.SelectedKeyName, bInDetectRange);
    Blackboard->SetValueAsBool(IsInAttackRangeKey.SelectedKeyName, bInAttackRange);

    DrawDebugCircle(
        GetWorld(),
        ControllPawn->GetActorLocation(),
        AttackRange,
        48,                               // 더 매끄럽게 48 정도로 수정
        FColor::Red,
        false,
        Interval + 0.1f,                  // [핵심] 서비스 실행 주기 + 알파 만큼 유지
        0,
        2.0f,
        FVector(1, 0, 0),                // X축
        FVector(0, 1, 0),                // Y축
        false                      // 채우기 여부
    );
    DrawDebugCircle(
        GetWorld(),
        ControllPawn->GetActorLocation(),
        Range,
        48,                               // 더 매끄럽게 48 정도로 수정
        FColor::Green,
        false,
        Interval + 0.1f,                  // [핵심] 서비스 실행 주기 + 알파 만큼 유지
        0,
        2.0f,
        FVector(1, 0, 0),                // X축
        FVector(0, 1, 0),                // Y축
        false                      // 채우기 여부
    );
    if (TargetEnemy)
    {
        DrawDebugLine(
            GetWorld(),
            ControllPawn->GetActorLocation(),
            TargetEnemy->GetActorLocation(),
            FColor::Yellow, // 타겟 연결선은 노란색
            false,
            Interval + 0.1f,
            0,
            1.5f
        );
    }

    if (AIC)
    {
        AIC->SetUnitState(bInDetectRange ? EUnitState::Combat : EUnitState::Move);
    }
} 
