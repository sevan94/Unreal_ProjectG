

#include "Mode/Save/PGSaveGame.h"


UPGSaveGame::UPGSaveGame()
{
    // 여기서 기본값을 초기화
    PlayerName = TEXT("Player");
    PlayerLevel = 1;
    PlayerGold = 1000;
    PlayerGem = 500;
    PlayerUnlock = 500;

    //사운드
    MasterVolume = 1.0f;
    BGMVolume = 1.0f;
    SFXVolume = 1.0f;
    VoiceVolume = 1.0f;
}