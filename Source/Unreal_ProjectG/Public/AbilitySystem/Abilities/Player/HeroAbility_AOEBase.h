// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/PGHeroGameplayAbility.h"
#include "HeroAbility_AOEBase.generated.h"

/**
 * 모든 AOE 어빌리티의 공통 베이스 클래스
 */
UCLASS()
class UNREAL_PROJECTG_API UHeroAbility_AOEBase : public UPGHeroGameplayAbility
{
	GENERATED_BODY()
	
public:
    UHeroAbility_AOEBase();

protected:
    virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
    virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

    // AOE 액터 스폰
    UFUNCTION(BlueprintCallable)
    AAOESkillActor* SpawnAndInitializeAOEActor(const FVector& SpawnLocation);

private:
    // 스킬이 적용할 모든 게임플레이 이펙트 스펙핸들을 빌드하는 헬퍼 함수
    TArray<FGameplayEffectSpecHandle> BuildAllSpecHandles();
    void AppendDamageSpecHandle(TArray<FGameplayEffectSpecHandle>& OutHandles);
    void AppendBuffDebuffSpecHandles(TArray<FGameplayEffectSpecHandle>& OutHandles);

protected:
    // Config 캐싱
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    FHeroAOECommonConfig AOEConfig;
};
