

#include "Mode/Save/PGSaveGame.h"


UPGSaveGame::UPGSaveGame()
{
    // 여기서 기본값을 초기화
    PlayerName = TEXT("Player");
    PlayerGold = 0;
    PlayerGem = 0;
    PlayerUnlock = 0;

    //사운드
    MasterVolume = 1.0f;
    BGMVolume = 1.0f;
    SFXVolume = 1.0f;
}