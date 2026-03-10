// Fill out your copyright notice in the Description page of Project Settings.


#include "PGGameplayTags.h"

namespace PGGameplayTags
{
    // 플레이어 어빌리티 태그
    UE_DEFINE_GAMEPLAY_TAG(Player_Ability_BasicAttack, "Player.Ability.BasicAttack");
    UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Skill, "Player.Ability.Skill");
    UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Die, "Player.Ability.Die");
    UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Initialize, "Player.Ability.Initialize");

    // 플레이어 무기 게임 플레이 태그
    UE_DEFINE_GAMEPLAY_TAG(Player_Weapon_Sword, "Player.Weapon.Sword");

    // 유닛 어빌리티 태그
    UE_DEFINE_GAMEPLAY_TAG(Unit_Ability_MagicCharge, "Unit.Ability.MagicCharge");
    UE_DEFINE_GAMEPLAY_TAG(Unit_Ability_ActivateSheild, "Unit.Ability.ActivateSheild");
    UE_DEFINE_GAMEPLAY_TAG(Unit_Ability_BasicAttack, "Unit.Ability.BasicAttack");
    UE_DEFINE_GAMEPLAY_TAG(Unit_Ability_SpecialAttack, "Unit.Ability.SpecialAttack");

    // 이벤트 태그
    UE_DEFINE_GAMEPLAY_TAG(Shared_Event_AttackTraceToggle, "Shared.Event.AttackTraceToggle");
    UE_DEFINE_GAMEPLAY_TAG(Shared_Event_ProjectileSpawn, "Shared.Event.ProjectileSpawn");
    UE_DEFINE_GAMEPLAY_TAG(Shared_Event_ActorSpawn, "Shared.Event.ActorSpawn");
    UE_DEFINE_GAMEPLAY_TAG(Shared_Event_MeleeHit, "Shared.Event.MeleeHit");
    UE_DEFINE_GAMEPLAY_TAG(Shared_Event_AOEExecute, "Shared.Event.AOEExecute");
    UE_DEFINE_GAMEPLAY_TAG(Shared_Event_HitReact, "Shared.Event.HitReact");

    // SetByCaller 태그
    UE_DEFINE_GAMEPLAY_TAG(Shared_SetByCaller_DamageMultiplier, "Shared.SetByCaller.DamageMultiplier");
    UE_DEFINE_GAMEPLAY_TAG(Shared_SetByCaller_MaxHealthMultiplier, "Shared.SetByCaller.MaxHealthMultiplier");
    UE_DEFINE_GAMEPLAY_TAG(Shared_SetByCaller_AttackPowerMultiplier, "Shared.SetByCaller.AttackPowerMultiplier");
    UE_DEFINE_GAMEPLAY_TAG(Shared_SetByCaller_BaseBuffAmount, "Shared.SetByCaller.BaseBuffAmount");
    UE_DEFINE_GAMEPLAY_TAG(Shared_SetByCaller_EffectDuration, "Shared.SetByCaller.EffectDuration");


    //유닛 등급 태그
    UE_DEFINE_GAMEPLAY_TAG(Unit_Tier_Normal, "Unit.Tier.Normal");
    UE_DEFINE_GAMEPLAY_TAG(Unit_Tier_Rare, "Unit.Tier.Rare");
    UE_DEFINE_GAMEPLAY_TAG(Unit_Tier_Epic, "Unit.Tier.Epic");

    //유닛 속성 태그
    UE_DEFINE_GAMEPLAY_TAG(Unit_Element_Fire, "Unit.Element.Fire");
    UE_DEFINE_GAMEPLAY_TAG(Unit_Element_Water, "Unit.Element.Water");
    UE_DEFINE_GAMEPLAY_TAG(Unit_Element_Electric, "Unit.Element.Electric");

    //유닛 병과 태그
    UE_DEFINE_GAMEPLAY_TAG(Unit_Branch_Melee, "Unit.Branch.Melee");
    UE_DEFINE_GAMEPLAY_TAG(Unit_Branch_Ranged, "Unit.Branch.Ranged");
    UE_DEFINE_GAMEPLAY_TAG(Unit_Branch_Mage, "Unit.Branch.Mage");
    UE_DEFINE_GAMEPLAY_TAG(Unit_Branch_Tank, "Unit.Branch.Tank");

    //유닛 진영 태그
    UE_DEFINE_GAMEPLAY_TAG(Unit_Side_Ally, "Unit.Side.Ally");
    UE_DEFINE_GAMEPLAY_TAG(Unit_Side_Foe, "Unit.Side.Foe");

    //유닛 상태(State) 태그
    UE_DEFINE_GAMEPLAY_TAG(Unit_State_Idle, "Unit.State.Idle");
    UE_DEFINE_GAMEPLAY_TAG(Unit_State_Combat, "Unit.State.Combat");
    UE_DEFINE_GAMEPLAY_TAG(Unit_State_Move, "Unit.State.Move");
    UE_DEFINE_GAMEPLAY_TAG(Unit_State_Dead, "Unit.State.Dead");
    UE_DEFINE_GAMEPLAY_TAG(Unit_State_Guard, "Unit.State.Guard");

    //버프 태그
    UE_DEFINE_GAMEPLAY_TAG(Buff_AttackPower, "Buff.AttackPower");
    UE_DEFINE_GAMEPLAY_TAG(Buff_MaxHealth, "Buff.MaxHealth");
}

