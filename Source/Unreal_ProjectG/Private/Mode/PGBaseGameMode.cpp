// Fill out your copyright notice in the Description page of Project Settings.

#include "Mode/PGBaseGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Pawn/BaseStructure.h"
#include "GameFramework/PlayerController.h"
#include "Mode/Save/PGGameInstance.h"
#include "AbilitySystem/PGCharacterAttributeSet.h"
#include "UI/Battle/BattleHUD.h"

APGBaseGameMode::APGBaseGameMode()
{
    PrimaryActorTick.bCanEverTick = true;
}

void APGBaseGameMode::BeginPlay()
{
    Super::BeginPlay();

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
            // 기지가 파괴되면 OnGameOver 함수가 실행되도록 연결
            Base->OnBaseDestroyed.AddDynamic(this, &APGBaseGameMode::OnGameOver);
        }
    }
}

void APGBaseGameMode::ShowStageResult(const FBattleResultData& ResultData)
{
    ABattleHUD* HUD = Cast<ABattleHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
    HUD->OnGameOver(ResultData);
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

    // 파괴된 팀이 'Enemy'라면 -> 플레이어 승리
    if (DefeatedTeam == ETeamType::Enemy)
    {
        Result.bIsVictory = true;

        // 클리어 시간 체크
        float PlayTime = GetCurrentPlayTime();
        UE_LOG(LogTemp, Warning, TEXT("Game Clear! PlayTime: %.2f sec"), PlayTime);

        if (PlayTime <= ClearTimeLimit_3Stars)
        {
            Result.StarCount = 3; // 3성 (빠른 클리어)
        }
        else if (PlayTime <= ClearTimeLimit_2Stars)
        {
            Result.StarCount = 2; // 2성 (보통)
        }
        else
        {
            Result.StarCount = 1; // 1성 (턱걸이)
        }

        UE_LOG(LogTemp, Warning, TEXT("클리어 등급: %d 성"), Result.StarCount);

        // 여기서 GameInstance를 불러와서 클리어 보상(골드 등)을 저장(Save)하는 로직을 추가 가능
    }
    else // 플레이어 기지 파괴 -> 패배
    {
        Result.bIsVictory = false;
        Result.StarCount = 0; // 패배 시 별 없음
        UE_LOG(LogTemp, Warning, TEXT("Game Over... Player Base Destroyed."));
    }

    ShowStageResult(Result);
}