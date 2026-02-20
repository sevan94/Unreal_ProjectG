// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/PGBaseGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Pawn/BaseStructure.h"
#include "GameFramework/PlayerController.h"

APGBaseGameMode::APGBaseGameMode()
{   
    PrimaryActorTick.bCanEverTick = true;
    
}


void APGBaseGameMode::BeginPlay()
{
    Super::BeginPlay(); 

    // 1. 게임 시작 시간 기록
    GameStartTime = GetWorld()->GetTimeSeconds();

    // 2. 맵에 배치된 모든 기지(BaseStructure)를 찾아서 파괴 이벤트 연결
    TArray<AActor*> FoundBases;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABaseStructure::StaticClass(), FoundBases);

    for (AActor* Actor : FoundBases)
    {
        ABaseStructure* Base = Cast<ABaseStructure>(Actor);
        if (Base)
        {
            // 기지가 파괴되면 OnGameOver 함수가 실행되도록 연결
            Base->OnBaseDestroyed.AddDynamic(this, &APGBaseGameMode::OnGameOver);
        }
    }
}

// --- 인구수 제한 로직 ---
bool APGBaseGameMode::CanSpawnUnit(ETeamType Team)
{
    if (Team == ETeamType::Ally)
    {
        // 아군 유닛이 MAX 미만일 때만 스폰 가능
        return CurrentAllyCount < MAX_ALLY_COUNT;
    }
    else if (Team == ETeamType::Enemy)
    {
        // 적군 유닛이 MAX 미만일 때만 스폰 가능
        return CurrentEnemyCount < MAX_ENEMY_COUNT;
    }
    return true;
}

void APGBaseGameMode::RegisterUnit(ETeamType Team)
{
    if (Team == ETeamType::Ally) CurrentAllyCount++;
    else if (Team == ETeamType::Enemy) CurrentEnemyCount++;
}

void APGBaseGameMode::UnregisterUnit(ETeamType Team)
{
    if (Team == ETeamType::Ally) CurrentAllyCount = FMath::Max(0, CurrentAllyCount - 1);
    else if (Team == ETeamType::Enemy) CurrentEnemyCount = FMath::Max(0, CurrentEnemyCount - 1);
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

    bool bIsPlayerVictory = false;
    int32 FinalStarCount = 0;

    // 파괴된 팀이 'Enemy'라면 -> 플레이어 승리
    if (DefeatedTeam == ETeamType::Enemy)
    {
        bIsPlayerVictory = true;

        // 클리어 시간 체크
        float PlayTime = GetCurrentPlayTime();
        UE_LOG(LogTemp, Warning, TEXT("Game Clear! PlayTime: %.2f sec"), PlayTime);

        if (PlayTime <= ClearTimeLimit_3Stars)
        {
            FinalStarCount = 3; // 3성 (빠른 클리어)
        }
        else if (PlayTime <= ClearTimeLimit_2Stars)
        {
            FinalStarCount = 2; // 2성 (보통)
        }
        else
        {
            FinalStarCount = 1; // 1성 (턱걸이)
        }
    }
    else // 플레이어 기지 파괴 -> 패배
    {
        bIsPlayerVictory = false;
        FinalStarCount = 0; // 패배 시 별 없음
        UE_LOG(LogTemp, Warning, TEXT("Game Over: Player Defeat"));
    }

    // 1. 결과 UI 호출 (BP_GameMode에서 위젯 생성)
    BP_ShowResultUI(bIsPlayerVictory, FinalStarCount);

    // 2. 플레이어 조작 비활성화 (선택 사항)
    APlayerController* PC = GetWorld()->GetFirstPlayerController();
    if (PC)
    {
        PC->SetCinematicMode(true, false, false, true, true); // 조작 차단
        PC->bShowMouseCursor = true; // 마우스 커서 보이기
    }
}