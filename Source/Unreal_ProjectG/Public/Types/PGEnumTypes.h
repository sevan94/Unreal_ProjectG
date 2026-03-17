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

// 플레이어 재화 열거형
UENUM()
enum class EGoodsCategory : uint8
{
    Gem         UMETA(DisplayName = "Gem"),
    Unlock      UMETA(DisplayName = "Unlock"),
    Gold        UMETA(DisplayName = "Gold")
};

// 영웅의 전투 모드를 나타내는 열거형
UENUM(BlueprintType)
enum class EHeroCombatMode : uint8
{
    None UMETA(DisplayName = "None"),
    Manual UMETA(DisplayName = "Manual"),
    Auto UMETA(DisplayName = "Auto"),
};

// AOE 공격의 실행 방식에 대한 열거형, 즉 즉시 효과인지 장판 형태로 남는 효과인지
UENUM(BlueprintType)
enum class EAOEExecutionType : uint8
{
    Instant,
    PersistentField,
};

// AOE 공격의 타겟팅 정책을 나타내는 열거형, 즉 적군만 타겟팅할지 아군만 타겟팅할지 등
UENUM(BlueprintType)
enum class EAOETargetPolicy : uint8
{
    HostileOnly,
    FriendlyOnly,
    AllExceptSelf,
};