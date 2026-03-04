// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/PGBaseGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Pawn/BaseStructure.h"
#include "GameFramework/PlayerController.h"
<<<<<<< Updated upstream
#include "AbilitySystem/PGCharacterAttributeSet.h"
=======
#include "Mode/Save/PGGameInstance.h"
>>>>>>> Stashed changes

APGBaseGameMode::APGBaseGameMode()
{   
    PrimaryActorTick.bCanEverTick = true;
    
}


void APGBaseGameMode::BeginPlay()
{
    Super::BeginPlay(); 

    // --- 스테이지 정보 세팅 시작 ---
    if (UPGGameInstance* GI = Cast<UPGGameInstance>(GetGameInstance()))
    {
        CurrentStageNum = GI->SelectedStageNum; // 로비에서 선택한 번호 가져오기

        // 만약 에디터에서 해당 스테이지 번호(예: 1~5)에 대한 데이터를 입력해 뒀다면?
        if (StageDataMap.Contains(CurrentStageNum))
        {
            FPGStageInfo CurrentStageInfo = StageDataMap[CurrentStageNum];

            // 게임 모드의 클리어 시간 제한을 해당 스테이지 난이도에 맞게 덮어쓰기
            ClearTimeLimit_3Stars = CurrentStageInfo.ClearTimeLimit_3Stars;
            ClearTimeLimit_2Stars = CurrentStageInfo.ClearTimeLimit_2Stars;

            UE_LOG(LogTemp, Warning, TEXT("=== 현재 스테이지: %d ==="), CurrentStageNum);
        }
    }
    // --- 스테이지 정보 세팅 끝 ---

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
            if (Base->GetTeamTag().MatchesTag(FGameplayTag::RequestGameplayTag(FName("Unit.Side.Ally"))))
            {
                AllyBase = Base;
            }
            // 기지가 파괴되면 OnGameOver 함수가 실행되도록 연결
            Base->OnBaseDestroyed.AddDynamic(this, &APGBaseGameMode::OnGameOver);
        }
    }
}

//// --- 인구수 제한 로직 ---
//bool APGBaseGameMode::CanSpawnUnit(ETeamType Team)
//{
//    if (Team == ETeamType::Ally)
//    {
//        // 아군 유닛이 MAX 미만일 때만 스폰 가능
//        return CurrentAllyCount < MAX_ALLY_COUNT;
//    }
//    else if (Team == ETeamType::Enemy)
//    {
//        // 적군 유닛이 MAX 미만일 때만 스폰 가능
//        return CurrentEnemyCount < MAX_ENEMY_COUNT;
//    }
//    return true;
//}
//
//void APGBaseGameMode::RegisterUnit(ETeamType Team)
//{
//    if (Team == ETeamType::Ally) CurrentAllyCount++;
//    else if (Team == ETeamType::Enemy) CurrentEnemyCount++;
//}
//
//void APGBaseGameMode::UnregisterUnit(ETeamType Team)
//{
//    if (Team == ETeamType::Ally) CurrentAllyCount = FMath::Max(0, CurrentAllyCount - 1);
//    else if (Team == ETeamType::Enemy) CurrentEnemyCount = FMath::Max(0, CurrentEnemyCount - 1);
//}

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
    Result.bIsVictory = (DefeatedTeam == ETeamType::Enemy);
    Result.TotalPlayTime = GetCurrentPlayTime();
    Result.TotalSpentCost = SpentCost;
    UPGCharacterAttributeSet* BaseAttribute = AllyBase->GetPGCharacterAttributeSet();

    if (Result.bIsVictory)
    {
        // 1. 별 개수 계산
        if (Result.TotalPlayTime <= ClearTimeLimit_3Stars) Result.StarCount = 3;
        else if (Result.TotalPlayTime <= ClearTimeLimit_2Stars) Result.StarCount = 2;
        else Result.StarCount = 1;

        // 아군 기지 체력 계산
        if (AllyBase) Result.RemainingHealthPercent = (BaseAttribute->GetHealth() / BaseAttribute->GetMaxHealth()) * 100.0f;
    }

    // 결과 UI 호출 (BP_GameMode에서 위젯 생성)
    BP_ShowResultUI(Result);
       

    // 2. 플레이어 조작 비활성화 (선택 사항)
    APlayerController* PC = GetWorld()->GetFirstPlayerController();
    if (PC)
    {
        PC->SetCinematicMode(true, false, false, true, true); // 조작 차단
        PC->bShowMouseCursor = true; // 마우스 커서 보이기
    }
}