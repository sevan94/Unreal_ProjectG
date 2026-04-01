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

//유닛 등급 열거형
UENUM(BlueprintType)
enum class EUnitRank : uint8
{
    Normal      UMETA(DisplayName = "Normal"),
    Rare        UMETA(DisplayName = "Rare"),
    SuperRare   UMETA(DisplayName = "SuperRare")
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

// AOE 공격의 타겟팅 정책을 나타내는 열거형, 즉 적군만 타겟팅할지 아군만 타겟팅할지 등
UENUM(BlueprintType)
enum class EAOETargetPolicy : uint8
{
    HostileOnly,
    FriendlyOnly,
    AllExceptSelf,
};

// 버프가 Infinite인지 Duration이 있는지 나타내는 열거형
UENUM(BlueprintType)
enum class EBuffDurationType : uint8
{
    Infinite,
    Duration
};

// 스킬의 발동 방식 (즉시 발동인지, 채널링인지 등)을 나타내는 열거형
UENUM(BlueprintType)
enum class ESkillActivationType : uint8
{
    Instant,
    Channeling,
    Passive
};

// 스테이지 달성도 타입 열거형
UENUM()
enum class ERewardCategory : uint8
{
    Time        UMETA(DisplayName = "Time"),
    Health      UMETA(DisplayName = "Health"),
    Cost        UMETA(DisplayName = "Cost")
};

UENUM(BlueprintType)
enum class ESkillTargetPolicy : uint8
{
    Enemy           UMETA(DisplayName = "적"),
    Ally            UMETA(DisplayName = "아군(자신 포함)"),
    AllyWithOutSelf UMETA(DisplayName = "아군(자신 제외)"),
    Self            UMETA(DisplayName = "자신")
};

UENUM(BlueprintType)
enum class EEffectType : uint8
{
    None                    UMETA(DisplayName = "없음"),
    Damage                  UMETA(DisplayName = "데미지"),
    Heal                    UMETA(DisplayName = "치유"),
    MaxHealthBuff           UMETA(DisplayName = "최대 체력 증가"),
    AttackPowerBuff         UMETA(DisplayName = "공격력 증가"),
    MoveSpeedBuff           UMETA(DisplayName = "이동 속도 증가"),
    AttackSpeedBuff         UMETA(DisplayName = "공격 속도 증가"),
    AttackPowerDebuff       UMETA(DisplayName = "공격력 감소"),
    MoveSpeedDebuff         UMETA(DisplayName = "이동 속도 감소"),
    AttackSpeedDebuff       UMETA(DisplayName = "공격 속도 감소"),
};