// Fill out your copyright notice in the Description page of Project Settings.

#include "Mode/PGBaseGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Pawn/BaseStructure.h"
#include "GameFramework/PlayerController.h"
#include "Mode/Save/PGGameInstance.h"

APGBaseGameMode::APGBaseGameMode()
{
    PrimaryActorTick.bCanEverTick = true;
}

void APGBaseGameMode::BeginPlay()
{
    Super::BeginPlay();

    // --- [1] 스테이지 정보 읽어오기 및 난이도 세팅 ---
    if (UPGGameInstance* GI = Cast<UPGGameInstance>(GetGameInstance()))
    {
        // 로비에서 선택하여 저장해둔 스테이지 번호를 가져옴. (없으면 기본값 1)
        CurrentStageNum = GI->SelectedStageNum;

        // 에디터에서 해당 스테이지 번호에 대한 데이터를 맵에 입력해 뒀다면 적용!
        if (StageDataMap.Contains(CurrentStageNum))
        {
            FPGStageInfo CurrentStageInfo = StageDataMap[CurrentStageNum];

            // 게임 모드의 클리어 시간 제한을 해당 스테이지 난이도에 맞게 덮어쓰기
            ClearTimeLimit_3Stars = CurrentStageInfo.ClearTimeLimit_3Stars;
            ClearTimeLimit_2Stars = CurrentStageInfo.ClearTimeLimit_2Stars;

            UE_LOG(LogTemp, Warning, TEXT("=== 현재 스테이지: %d ==="), CurrentStageNum);
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("스테이지 %d에 대한 데이터가 StageDataMap에 없습니다. 기본값을 사용합니다."), CurrentStageNum);
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
            // 기지가 파괴되면 OnGameOver 함수가 실행되도록 연결
            Base->OnBaseDestroyed.AddDynamic(this, &APGBaseGameMode::OnGameOver);
        }
    }
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

        UE_LOG(LogTemp, Warning, TEXT("클리어 등급: %d 성"), FinalStarCount);

        // 여기서 GameInstance를 불러와서 클리어 보상(골드 등)을 저장(Save)하는 로직을 추가 가능
    }
    else // 플레이어 기지 파괴 -> 패배
    {
        bIsPlayerVictory = false;
        FinalStarCount = 0; // 패배 시 별 없음
        UE_LOG(LogTemp, Warning, TEXT("Game Over... Player Base Destroyed."));
    }
}