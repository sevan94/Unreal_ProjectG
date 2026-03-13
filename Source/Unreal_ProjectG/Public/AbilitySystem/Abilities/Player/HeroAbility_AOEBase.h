// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/PGHeroGameplayAbility.h"
#include "HeroAbility_AOEBase.generated.h"

/**
 * 모든 AOE 어빌리티의 공통 베이스 클래스
 * 광역 공격은 대상 지정 방식과 발동 방식이 다양하기 때문에, 공통적으로 사용할 기능과 인터페이스를 이 클래스에 정의한다.
 * 광역 공격은 액터를 통하여 구현하고 이 어빌리티는 이펙트 캡처만 담당하여 액터에 이펙트 스펙 핸들을 전달한다.
 * 따라서 실제 데미지 정보는 이 어빌리티가 아닌 액터에 있다.
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
    FGameplayEffectSpecHandle MakeBuffDebuffSpecHandle();
    FGameplayEffectSpecHandle MakeDamageSpecHandle();
    TArray<FGameplayEffectSpecHandle> MakeStatusSpecHandles();
protected:
    // Config 캐싱
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    FHeroAOECommonConfig AOEConfig;
};
