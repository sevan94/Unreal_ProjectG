// Fill out your copyright notice in the Description page of Project Settings.

#include "Mode/PGBaseGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Pawn/BaseStructure.h"
#include "GameFramework/PlayerController.h"
#include "Mode/Save/PGGameInstance.h"
#include "AbilitySystem/PGCharacterAttributeSet.h"
#include "UI/Battle/BattleHUD.h"
#include "Character/Unit/SubSystem/UnitSpawnSubsystem.h"
#include "Character/HeroController.h"
#include "DataAssets/UI/UnitUIDataAsset.h"

APGBaseGameMode::APGBaseGameMode()
{
    PrimaryActorTick.bCanEverTick = true;
}

void APGBaseGameMode::BeginPlay()
{
    Super::BeginPlay();

    if (UWorld* World = GetWorld())
    {
        if (UUnitSpawnSubsystem* SpawnSystem = World->GetSubsystem<UUnitSpawnSubsystem>())
        {
            if (UPGGameInstance* GI = Cast<UPGGameInstance>(GetGameInstance()))
            {
                for (TSoftObjectPtr<UUnitUIDataAsset> SoftUnitData : GI->CurrentUnits)
                {
                    if (UUnitUIDataAsset* LoadedUnitData = SoftUnitData.LoadSynchronous())
                    {
                        if (LoadedUnitData->UnitClass)
                        {
                            SpawnSystem->PrewarmPool(LoadedUnitData->UnitClass, 5);
                        }
                    }
                }
            }
        }
    }

    // --- [2] 게임 시작 시간 기록 ---
    GameStartTime = GetWorld()->GetTimeSeconds();

    // --- [3] 맵에 배치된 모든 기지(BaseStructure)를 찾아서 파괴 이벤트 연결 ---
    TArray<AActor*> FoundBases;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABaseStructure::StaticClass(), FoundBases);

    for (AActor* Actor : FoundBases)
    {
        ABaseStructure* Base = Cast<ABaseStructure>(Actor);
        if (Base)
        {
            if (Base->GetTeamTag().MatchesTag(FGameplayTag::RequestGameplayTag(FName("Unit.Side.Ally"))))
            {
                AllyBase = Base;
            }
            if (Base->GetTeamTag().MatchesTag(FGameplayTag::RequestGameplayTag(FName("Unit.Side.Foe"))))
            {
                EnemyBase = Base;
            }
            // 기지가 파괴되면 OnGameOver 함수가 실행되도록 연결
            Base->OnBaseDestroyed.AddDynamic(this, &APGBaseGameMode::OnGameOver);
        }
    }

    // 카메라 이동 범위 제한
    AHeroController* PC = Cast<AHeroController>(GetWorld()->GetFirstPlayerController());
    if (PC)
    {
        PC->SetCameraClamp(AllyBase->GetActorLocation().X + 200.0f, EnemyBase->GetActorLocation().X - 200.0f);
    }
}

void APGBaseGameMode::ShowStageResult(const FBattleResultData& ResultData)
{
    ABattleHUD* HUD = Cast<ABattleHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
    HUD->OnGameOver(ResultData);
}

void APGBaseGameMode::SetStageResult()
{
    UPGGameInstance* GI = Cast<UPGGameInstance>(GetGameInstance());
    if (GI)
    {
        int32 FinalStarCount = 1; // 기본 클리어 1성
        float CurrentValue = 0.0f;

        // 카테고리에 따른 현재 기록 측정
        switch (GI->CurrentStageData.RewardType)
        {
        case ERewardCategory::Time:
        {
            CurrentValue = GetCurrentPlayTime(); // 소요 시간
            break;
        }
        case ERewardCategory::Health:
        {
            if (AllyBase)
            {
                UPGCharacterAttributeSet* BaseAttribute = AllyBase->GetPGCharacterAttributeSet();
                CurrentValue = (BaseAttribute->GetHealth() / BaseAttribute->GetMaxHealth()) * 100.0f;
            }
            break;
        }
        case ERewardCategory::Cost:
        {
            CurrentValue = SpentCost; // 전투 중 소모한 총 코스트
            break;
        }
        }

        // 별 개수 판별 (시간/코스트는 작을수록 좋고, 체력은 클수록 좋음)
        bool bSetStarCount = (GI->CurrentStageData.RewardType != ERewardCategory::Health);

        if (bSetStarCount) {
            if (CurrentValue <= GI->CurrentStageData.Star3) FinalStarCount = 3;
            else if (CurrentValue <= GI->CurrentStageData.Star2) FinalStarCount = 2;
        }
        else {
            if (CurrentValue >= GI->CurrentStageData.Star3) FinalStarCount = 3;
            else if (CurrentValue >= GI->CurrentStageData.Star2) FinalStarCount = 2;
        }

        // 결과 저장 및 보상 지급 호출
        GI->UpdateStageClearData(GI->CurrentStageData.StageCode, FinalStarCount);
    }
}

int32 APGBaseGameMode::CalculateStarCount(const FBattleResultData& ResultData)
{
    UPGGameInstance* GI = Cast<UPGGameInstance>(GetGameInstance());
    if (!GI) return 1;

    float CurrentValue = 0.0f;
    // 리워드 타입에 따른 현재 값 설정
    switch (GI->CurrentStageData.RewardType)
    {
    case ERewardCategory::Time:   CurrentValue = ResultData.TotalPlayTime; break;
    case ERewardCategory::Health: CurrentValue = ResultData.RemainingHealthPercent; break;
    case ERewardCategory::Cost:   CurrentValue = ResultData.TotalSpentCost; break;
    }

    // 조건 비교
    bool bLowerIsBetter = (GI->CurrentStageData.RewardType != ERewardCategory::Health);
    if (bLowerIsBetter)
    {
        if (CurrentValue <= GI->CurrentStageData.Star3) return 3;
        if (CurrentValue <= GI->CurrentStageData.Star2) return 2;
    }
    else
    {
        if (CurrentValue >= GI->CurrentStageData.Star3) return 3;
        if (CurrentValue >= GI->CurrentStageData.Star2) return 2;
    }

    return 1;
}

// --- 시간 및 등급 관리 ---
float APGBaseGameMode::GetCurrentPlayTime() const
{
    // 현재 서버 시간 - 게임 시작 시간
    return GetWorld()->GetTimeSeconds() - GameStartTime;
}

// --- 승패 판정 로직 ---
void APGBaseGameMode::OnGameOver(ETeamType DefeatedTeam)
{
    if (bIsGameOver) return; // 이미 종료된 게임이면 무시
    bIsGameOver = true;

    FBattleResultData Result;

    Result.bIsVictory = false;
    Result.StarCount = 0;
    Result.TotalPlayTime = GetCurrentPlayTime();
    Result.TotalSpentCost = SpentCost;

    if (AllyBase)
    {
        UPGCharacterAttributeSet* BaseAttribute = AllyBase->GetPGCharacterAttributeSet();
        Result.RemainingHealthPercent = (BaseAttribute->GetHealth() / BaseAttribute->GetMaxHealth()) * 100.0f;
    }

    if (DefeatedTeam == ETeamType::Enemy) // 플레이어 승리
    {
        Result.bIsVictory = true;

        UPGGameInstance* GI = Cast<UPGGameInstance>(GetGameInstance());
        if (GI)
        {
            // 최초 클리어 여부 판정
            int32* OldStars = GI->StageClearData.Find(GI->CurrentStageData.StageCode);
            Result.bIsFirstClear = (OldStars == nullptr || *OldStars == 0);

            // 별 개수 계산
            Result.StarCount = CalculateStarCount(Result);

            // 스테이지 클리어 데이터 업데이트
            GI->UpdateStageClearData(GI->CurrentStageData.StageCode, Result.StarCount);
        }
    }
    else // 플레이어 기지 파괴 -> 패배
    {
        Result.bIsVictory = false;
        Result.StarCount = 0; // 패배 시 별 없음
        Result.bIsFirstClear = false;
        UE_LOG(LogTemp, Warning, TEXT("Game Over... Player Base Destroyed."));
    }

    ShowStageResult(Result);
}