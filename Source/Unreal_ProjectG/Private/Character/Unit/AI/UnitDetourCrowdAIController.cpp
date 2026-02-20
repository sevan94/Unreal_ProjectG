// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Unit/AI/UnitDetourCrowdAIController.h"
#include "Character/Unit/UnitCharacter.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Navigation/CrowdFollowingComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"

AUnitDetourCrowdAIController::AUnitDetourCrowdAIController(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>(TEXT("PathFollowingComponent")))
{
    BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComp"));
}

void AUnitDetourCrowdAIController::OnPossess(APawn* InPawn)
{
    UE_LOG(LogTemp, Log, TEXT("OnPossess"));

    /*지금 방식은 비동기 로딩이 끝나기 전에 빙의하기 때문에 작동 안될 가능성이 높음.
    제미나이 물어보니 델리게이트를 이용하라 하는데 이건 내일 한번 상의해야 할듯???*/
    
    Super::OnPossess(InPawn);

    UCrowdFollowingComponent* CrowdComp = Cast<UCrowdFollowingComponent>(GetPathFollowingComponent());
    if (CrowdComp)
    {
        CrowdComp->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::High);
    }

    AUnitCharacter* Unit = Cast<AUnitCharacter>(InPawn);
    if (Unit)
    {
        Unit->OnUnitStartUpDataLoadedDelegate.AddUObject(this, &AUnitDetourCrowdAIController::InitializeAI);
    }

}

void AUnitDetourCrowdAIController::OnUnPossess()
{
    if (UBehaviorTreeComponent* BTComp = Cast<UBehaviorTreeComponent>(BrainComponent))
    {
        BTComp->StopTree(EBTStopMode::Safe);
    }

    AUnitCharacter* Unit = Cast<AUnitCharacter>(GetPawn());
    if (Unit)
    {
        Unit->OnUnitStartUpDataLoadedDelegate.RemoveAll(this);
    }

    Super::OnUnPossess();
}

FGenericTeamId AUnitDetourCrowdAIController::GetGenericTeamId() const
{
    return FGenericTeamId();
}

void AUnitDetourCrowdAIController::InitializeAI()
{
    AUnitCharacter* CrowdUnit = Cast<AUnitCharacter>(GetPawn());

    if (CrowdUnit && BlackboardComp)
    {
        BlackboardComp->SetValueAsFloat(TEXT("DetectRange"), CrowdUnit->GetDetectRangeKey());
        BlackboardComp->SetValueAsFloat(TEXT("AttackRange"), CrowdUnit->GetAttackRangeKey());
        BlackboardComp->SetValueAsFloat(TEXT("AttackMargin"), CrowdUnit->GetAttackMarginKey());

        if (UBehaviorTree* SubTree = CrowdUnit->GetSubBTAssetKey())
        {
            BlackboardComp->SetValueAsObject(TEXT("SubBT"), SubTree);
        }

        if (BTAsset)
        {
            if (RunBehaviorTree(BTAsset))
            {
                UE_LOG(LogTemp, Log, TEXT("메인 BT 실행 성공"));
                SetUnitState(EUnitState::Move);
                if (CrowdUnit->TargetActor)
                {
                    BlackboardComp->SetValueAsObject(TEXT("AttackTargetBase"), CrowdUnit->TargetActor);
                    UE_LOG(LogTemp, Log, TEXT("AttackTargetBase설정완"));

                }
                UBehaviorTreeComponent* BTComp = Cast<UBehaviorTreeComponent>(BrainComponent);
                if (BTComp)
                {
                    //나중에 pggameplaytag 사용하게 수정하면 조음-
                    FGameplayTag CombatTag = FGameplayTag::RequestGameplayTag(TEXT("Unit.State.Combat"));
                }
            }
            else
            {
                UE_LOG(LogTemp, Error, TEXT("FailRunBehaviorTree"));
            }
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("BTAssetNull"));
        }
    }
}

void AUnitDetourCrowdAIController::SetUnitState(EUnitState NewState)
{
    if (BlackboardComp)
    {
        Blackboard->SetValueAsEnum(StateKeyName, static_cast<uint8>(NewState));
        UCrowdFollowingComponent* CrowdComp = Cast<UCrowdFollowingComponent>(GetPathFollowingComponent());
        if (CrowdComp)
        {
            if (NewState == EUnitState::Combat)
            {
                //CrowdComp->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::Low);

            }
            else if (NewState == EUnitState::Dead)
            {
                CrowdComp->SetCrowdSimulationState(ECrowdSimulationState::Disabled);
            }
            else
            {
                CrowdComp->SetCrowdSimulationState(ECrowdSimulationState::Enabled);
                CrowdComp->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::High);
            }
        }
    }
}
