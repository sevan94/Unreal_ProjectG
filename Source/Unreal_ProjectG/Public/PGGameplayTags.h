// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "NativeGameplayTags.h"

//
// 태그를 정의하는 곳
//
namespace PGGameplayTags
{
    // 인풋 태그
    UNREAL_PROJECTG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_BaseAttack);
    UNREAL_PROJECTG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Skill_1);
    UNREAL_PROJECTG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Skill_2);

    // 플레이어 어빌리티 태그
    UNREAL_PROJECTG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Ability_BasicAttack_Melee);
    UNREAL_PROJECTG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Ability_BasicAttack_Projectile);
    UNREAL_PROJECTG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Ability_Skill_1);
    UNREAL_PROJECTG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Ability_Skill_2);

    // 플레이어 무기 게임 플레이 태그 
    UNREAL_PROJECTG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Weapon_Sword);

    // 플레이어 이벤트 태그

    // 유닛 어빌리티 태그
    UNREAL_PROJECTG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Unit_Ability_BasicAttack_Melee);

    // 공유 이벤트 태그
    UNREAL_PROJECTG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_Event_AttackTraceToggle); // 몽타주에서 공격 트레이스 온오프 토글용
    UNREAL_PROJECTG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_Event_ProjectileSpawn);
    UNREAL_PROJECTG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_Event_MeleeHit);
    UNREAL_PROJECTG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_Event_AOEExecute);

    // 공유 SetByCaller 태그
    UNREAL_PROJECTG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_SetByCaller_DamageMultiplier);
    UNREAL_PROJECTG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_SetByCaller_MaxHealthMultiplier);
    UNREAL_PROJECTG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_SetByCaller_AttackPowerMultiplier);


    //유닛 상태(State) 태그
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Unit_State_Idle);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Unit_State_Combat);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Unit_State_Move);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Unit_State_Dead);


    //유닛 등급 태그
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Unit_Tier_Normal);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Unit_Tier_Rare);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Unit_Tier_Epic);

    //유닛 속성 태그
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Unit_Element_Fire);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Unit_Element_Water);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Unit_Element_Electric);

    //유닛 병과 태그
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Unit_Branch_Melee);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Unit_Branch_Ranged);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Unit_Branch_Mage);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Unit_Branch_Tank);

    //아군 적군 구분 태그
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Unit_Side_Ally);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Unit_Side_Foe);

    //게임 플레이 큐 태그
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayCue_VFX_AOEImpact);

}
