// Fill out your copyright notice in the Description page of Project Settings.


#include "PGGameplayTags.h"

namespace PGGameplayTags
{
    // 인풋 태그
    UE_DEFINE_GAMEPLAY_TAG(Input_BasicAttack, "Input.BasicAttack");
    UE_DEFINE_GAMEPLAY_TAG(Input_ActiveSkill, "Input.ActiveSkill");
    UE_DEFINE_GAMEPLAY_TAG(Input_ActiveSkill_SubSkill, "Input.ActiveSkill.SubSkill");
    UE_DEFINE_GAMEPLAY_TAG(Input_ActiveSkill_MainSkill, "Input.ActiveSkill.MainSkill");

    // 플레이어 상태 태그
    UE_DEFINE_GAMEPLAY_TAG(State_InputBlock_Move, "State.InputBlock.Move");
    UE_DEFINE_GAMEPLAY_TAG(State_InputBlock_ScreenTouch, "State.InputBlock.ScreenTouch");

    // 플레이어 어빌리티 태그
    UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Combat_BasicAttack, "Player.Ability.Combat.BasicAttack");
    UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Combat_Skill, "Player.Ability.Combat.Skill");
    UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Combat_Skill_SubSkill, "Player.Ability.Combat.Skill.SubSkill");
    UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Combat_Skill_MainSkill, "Player.Ability.Combat.Skill.MainSkill");
    UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Die, "Player.Ability.Die");
    UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Initialize, "Player.Ability.Initialize");

    // 어빌리티 쿨다운 태그
    UE_DEFINE_GAMEPLAY_TAG(Ability_Cooldown_MainSkill, "Ability.Cooldown.MainSkill");
    UE_DEFINE_GAMEPLAY_TAG(Ability_Cooldown_SubSkill, "Ability.Cooldown.SubSkill");

    // 플레이어 세트 게임 플레이 태그 
    UE_DEFINE_GAMEPLAY_TAG(Equipment_Set_ChainSword, "Equipment.Set.ChainSword");

    UE_DEFINE_GAMEPLAY_TAG(Equipment_Set_RockStar, "Equipment.Set.RockStar");
    UE_DEFINE_GAMEPLAY_TAG(Equipment_Set_DevilBat, "Equipment.Set.DevilBat");

    UE_DEFINE_GAMEPLAY_TAG(Equipment_Set_Dimension, "Equipment.Set.Dimension");
    UE_DEFINE_GAMEPLAY_TAG(Equipment_Set_Healer, "Equipment.Set.Healer");
    UE_DEFINE_GAMEPLAY_TAG(Equipment_Set_FireballMaster, "Equipment.Set.FireballMaster");

    // 이벤트 트리거
    UE_DEFINE_GAMEPLAY_TAG(Event_Trigger_OnHit, "Event.Trigger.OnHit");
    UE_DEFINE_GAMEPLAY_TAG(Event_Trigger_OnKill, "Event.Trigger.OnKill");
    UE_DEFINE_GAMEPLAY_TAG(Event_Trigger_OnCommit, "Event.Trigger.OnCommit");
    UE_DEFINE_GAMEPLAY_TAG(Event_Trigger_OnSkillPresentation, "Event.Trigger.OnSkillPresentation");
    UE_DEFINE_GAMEPLAY_TAG(Event_Trigger_OnTargetDataReady, "Event.Trigger.OnTargetDataReady");

    // 어트리뷰트 태그(버프/디버프 구분 등에 사용)
    UE_DEFINE_GAMEPLAY_TAG(Attribute_Health, "Attribute.Health");
    UE_DEFINE_GAMEPLAY_TAG(Attribute_MaxHealth, "Attribute.MaxHealth");
    UE_DEFINE_GAMEPLAY_TAG(Attribute_Cost, "Attribute.Cost");
    UE_DEFINE_GAMEPLAY_TAG(Attribute_AttackPower, "Attribute.AttackPower");
    UE_DEFINE_GAMEPLAY_TAG(Attribute_MoveSpeed, "Attribute.MoveSpeed");

    // 유닛 어빌리티 태그
    UE_DEFINE_GAMEPLAY_TAG(Unit_Ability_MagicCharge, "Unit.Ability.MagicCharge");
    UE_DEFINE_GAMEPLAY_TAG(Unit_Ability_Buff, "Unit.Ability.Buff");
    UE_DEFINE_GAMEPLAY_TAG(Unit_Ability_BasicAttack, "Unit.Ability.BasicAttack");
    UE_DEFINE_GAMEPLAY_TAG(Unit_Ability_SpecialAttack, "Unit.Ability.SpecialAttack");

    // 이벤트 태그
    UE_DEFINE_GAMEPLAY_TAG(Shared_Event_MeleeTraceStart, "Shared.Event.MeleeTraceStart");
    UE_DEFINE_GAMEPLAY_TAG(Shared_Event_MeleeTraceEnd, "Shared.Event.MeleeTraceEnd");
    UE_DEFINE_GAMEPLAY_TAG(Shared_Event_ProjectileSpawn, "Shared.Event.ProjectileSpawn");
    UE_DEFINE_GAMEPLAY_TAG(Shared_Event_ActorSpawn, "Shared.Event.ActorSpawn");
    UE_DEFINE_GAMEPLAY_TAG(Shared_Event_ActorDestroy, "Shared.Event.ActorDestroy");
    UE_DEFINE_GAMEPLAY_TAG(Shared_Event_MeleeHit, "Shared.Event.MeleeHit");
    UE_DEFINE_GAMEPLAY_TAG(Shared_Event_AOEExecute, "Shared.Event.AOEExecute");
    UE_DEFINE_GAMEPLAY_TAG(Shared_Event_HitReact, "Shared.Event.HitReact");
    UE_DEFINE_GAMEPLAY_TAG(Shared_Event_SupportExecute, "Shared.Event.SupportExecute");

    // SetByCaller 태그
    UE_DEFINE_GAMEPLAY_TAG(Shared_SetByCaller_SkillMultiplier, "Shared.SetByCaller.SkillMultiplier");
    UE_DEFINE_GAMEPLAY_TAG(Shared_SetByCaller_BaseAmount, "Shared.SetByCaller.BaseAmount");
    UE_DEFINE_GAMEPLAY_TAG(Shared_SetByCaller_Duration, "Shared.SetByCaller.Duration");
    UE_DEFINE_GAMEPLAY_TAG(Shared_SetByCaller_BaseBuffAmount, "Shared.SetByCaller.BaseBuffAmount");

     // 스탯 관련 SetByCaller 태그
    UE_DEFINE_GAMEPLAY_TAG(Attribute_SetByCaller_MaxHealth, "Attribute.SetByCaller.MaxHealth");
    UE_DEFINE_GAMEPLAY_TAG(Attribute_SetByCaller_AttackPower, "Attribute.SetByCaller.AttackPower");
    UE_DEFINE_GAMEPLAY_TAG(Attribute_SetByCaller_MoveSpeed, "Attribute.SetByCaller.MoveSpeed");

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
    UE_DEFINE_GAMEPLAY_TAG(Unit_Branch_Support, "Unit.Branch.Support");

    //유닛 진영 태그
    UE_DEFINE_GAMEPLAY_TAG(Unit_Side_Ally, "Unit.Side.Ally");
    UE_DEFINE_GAMEPLAY_TAG(Unit_Side_Foe, "Unit.Side.Foe");

    //유닛 상태(State) 태그
    UE_DEFINE_GAMEPLAY_TAG(Unit_State_Idle, "Unit.State.Idle");
    UE_DEFINE_GAMEPLAY_TAG(Unit_State_Combat, "Unit.State.Combat");
    UE_DEFINE_GAMEPLAY_TAG(Unit_State_Move, "Unit.State.Move");
    UE_DEFINE_GAMEPLAY_TAG(Unit_State_Dead, "Unit.State.Dead");
    UE_DEFINE_GAMEPLAY_TAG(Unit_State_Guard, "Unit.State.Guard");
    UE_DEFINE_GAMEPLAY_TAG(Unit_State_Invincible, "Unit.State.Invincible");

    //유닛 버프 디버프 태그
    UE_DEFINE_GAMEPLAY_TAG(GameplayCue_VFX_Unit_Dbuff_Poison, "GameplayCue.VFX.Unit.Dbuff.Poison");
    UE_DEFINE_GAMEPLAY_TAG(GameplayCue_VFX_Unit_Dbuff_Slow, "GameplayCue.VFX.Unit.Dbuff.Slow");
    UE_DEFINE_GAMEPLAY_TAG(GameplayCue_VFX_Unit_Dbuff_Weekness, "GameplayCue.VFX.Unit.Dbuff.Weekness");
    UE_DEFINE_GAMEPLAY_TAG(GameplayCue_VFX_Unit_Dbuff_Frozen, "GameplayCue.VFX.Unit.Dbuff.Frozen");
    UE_DEFINE_GAMEPLAY_TAG(GameplayCue_VFX_Unit_Buff_Boost, "GameplayCue.VFX.Unit.Buff.Boost");
    UE_DEFINE_GAMEPLAY_TAG(GameplayCue_VFX_Unit_Buff_berserk, "GameplayCue.VFX.Unit.Buff.berserk");
    UE_DEFINE_GAMEPLAY_TAG(GameplayCue_VFX_Unit_Buff_Warth, "GameplayCue.VFX.Unit.Buff.Warth");
    UE_DEFINE_GAMEPLAY_TAG(GameplayCue_VFX_Unit_Buff_Clam, "GameplayCue.VFX.Unit.Buff.Clam");
    UE_DEFINE_GAMEPLAY_TAG(GameplayCue_VFX_Unit_Buff_Divinity, "GameplayCue.VFX.Unit.Buff.Divinity");

    UE_DEFINE_GAMEPLAY_TAG(GameplayCue_SVFX_Static, "GameplayCue.SVFX.Static");

    UE_DEFINE_GAMEPLAY_TAG(GameplayCue_SVFX_Actor, "GameplayCue.SVFX.Actor");
}
