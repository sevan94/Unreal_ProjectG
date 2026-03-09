// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// 유효성 여부 타입
UENUM()
enum class EPGValidType : uint8
{
    Valid,
    InValid
};

UENUM()
enum class EPGSuccessType : uint8
{
    Successful,
    Failed
};

UENUM()
enum class EPGToggleType : uint8
{
    On,
    Off,
    Toggle
};

// --- 팀 구분 (피아 식별용) ---
// 블루프린트 에디터에서 드롭다운으로 선택 가능
UENUM(BlueprintType)
enum class ETeamType : uint8
{
    None        UMETA(DisplayName = "None"),
    Player      UMETA(DisplayName = "Player Hero"), // 플레이어 영웅
    Ally        UMETA(DisplayName = "Ally Unit"),   // 아군 소환수
    Enemy       UMETA(DisplayName = "Enemy Unit"),  // 적군
    Neutral     UMETA(DisplayName = "Neutral")      // 중립 (오브젝트 등)
};

//유닛 상태를 나타내는 열거형
UENUM(BlueprintType)
enum class EUnitState : uint8
{
    Idle        UMETA(DisplayName = "Idle"),
    Combat      UMETA(DisplayName = "Combat"),
    Move        UMETA(DisplayName = "Move"),
    Dead        UMETA(DisplayName = "Dead")
};


// UI에서 장비 구분용 열거형
UENUM()
enum class EEquipCategory : uint8
{
    Weapon      UMETA(DisplayName = "Weapon"),
    Armor       UMETA(DisplayName = "Armor"),
    Accessory   UMETA(DisplayName = "Accessory")
};
