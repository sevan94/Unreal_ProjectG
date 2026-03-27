#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/PGHeroGameplayAbility.h"
#include "DataAssets/Ability/DataAsset_HeroSkillData.h"
#include "PGHeroSkillGameplayAbility.generated.h"

UENUM()
enum class EHeroSkillType : uint8
{
    None            UMETA(DisplayName = "없음"),
    MainSkill       UMETA(DisplayName = "메인 스킬"),
    SubSkill        UMETA(DisplayName = "서브 스킬"),
    BasicAttack     UMETA(DisplayName = "기본 공격"),
};

UCLASS()
class UNREAL_PROJECTG_API UPGHeroSkillGameplayAbility : public UPGHeroGameplayAbility
{
    GENERATED_BODY()

public:
    UPGHeroSkillGameplayAbility();

protected:
    virtual void ActivateAbility(
        const FGameplayAbilitySpecHandle Handle,
        const FGameplayAbilityActorInfo* ActorInfo,
        const FGameplayAbilityActivationInfo ActivationInfo,
        const FGameplayEventData* TriggerEventData) override;

    virtual void EndAbility(
        const FGameplayAbilitySpecHandle Handle,
        const FGameplayAbilityActorInfo* ActorInfo,
        const FGameplayAbilityActivationInfo ActivationInfo,
        bool bReplicateEndAbility,
        bool bWasCancelled) override;

    // 쿨다운 게임플레이 이펙트에서 태그를 가져오던 것을 이 어빌리티의 태그를 보고 메인 스킬인지 서브 스킬인지 판단해서 적절한 태그를 반환하도록 변경
    virtual const FGameplayTagContainer* GetCooldownTags() const override;
    // 스킬데이터의 쿨타임 Value와 GetCooldownTags()로 반환된 태그로 이펙트 스펙을 만들어서 적용하도록 변경
    virtual void ApplyCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const override;

private:
    void ExecuteNextAction();

    void ExecuteEventTrigger(EHeroSkillEventTrigger Trigger, const FGameplayTag& SourceEventTag, const FGameplayAbilityTargetDataHandle& TargetData);
    void ExecuteEventAction(const FSkillActionRow& ActionRow, const FGameplayAbilityTargetDataHandle& TargetData);

    static bool ConvertEventTagToTrigger(const FGameplayTag& EventTag, EHeroSkillEventTrigger& OutTrigger);

    UFUNCTION()
    void OnTaskRuntimeEvent(FGameplayTag EventTag, FGameplayAbilityTargetDataHandle TargetData);

    UFUNCTION()
    void OnActionCompleted(FGameplayAbilityTargetDataHandle TargetData);

    UFUNCTION()
    void OnActionCancelled(FGameplayAbilityTargetDataHandle TargetData);

    UFUNCTION()
    EHeroSkillType GetHeroSkillType() const;

private:
    int32 CurrentActionIndex = 0;

    UPROPERTY()
    TObjectPtr<UDataAsset_HeroSkillData> SkillData = nullptr;

    UPROPERTY()
    TArray<FSkillActionRow> RuntimeActionSequence;

    bool bAutoMode = false;

    FGameplayTagContainer TempCooldownTags;
    bool bIsCooltimeTagsInitialized = false;
};
