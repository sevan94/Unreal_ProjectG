#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayEffectTypes.h"
#include "Types/PGEnumTypes.h"
#include "ScalableFloat.h"
#include "AOESkillActor.generated.h"

class USphereComponent;

USTRUCT(BlueprintType)
struct FBuffDebuffEffectConfig
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Categories = "Attribute_SetByCaller"))
    FGameplayTag BuffTag; // 버프/디버프의 태그, 예: "Buff.AttackPower", "Debuff.Defense"
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FScalableFloat SkillMultiplier;
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FScalableFloat BaseBuffAmount;
};

/**
* AOE 어빌리티가 스폰하는 액터
* 나이아가라와 실제 게임플레이 이펙트를 적용하는 액터
* 블루프린트에서 CollectionAndApplyEffects 함수를 호출하여, CollisionSphere에 겹쳐진 액터들에게 EffectSpecHandles에 담긴 이펙트들을 적용
*/

UCLASS()
class UNREAL_PROJECTG_API AAOESkillActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AAOESkillActor();

    void InitializeAOEActor(AActor* InInstigator, EAOETargetPolicy InTargetPolicy, int32 InAbilityLevel, const FGameplayEffectSpecHandle& InDamageEffectSpecHandle, const FGameplayEffectSpecHandle& InBuffDebuffEffectSpecHandle, const TArray<FGameplayEffectSpecHandle>& InStatusEffectSpecHandles);

    UFUNCTION(BlueprintPure)
    float GetAOERadius() const;

    UFUNCTION(BlueprintCallable, meta = (DisplayName = "CollectAndApplyEffects"))
    void BP_CollectAndApplyEffects();

protected:
    UFUNCTION(BlueprintPure)
    FGameplayEffectSpecHandle MakeDamageEffectSpecHandleInAOE(float InSkillMultiplier) const;

    UFUNCTION(BlueprintPure)
    FGameplayEffectSpecHandle MakeBuffDebuffEffectSpecHandleInAOE(FBuffDebuffEffectConfig& InEffectConfig) const;

    UFUNCTION(BlueprintPure)
    TArray<FGameplayEffectSpecHandle> MakeStatusEffectSpecHandlesInAOE(float InDuration) const;

private:
    bool IsValidTarget(AActor* InTarget) const;
    void ApplySpecHandlesToSingleTarget(AActor* InTarget);

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    TObjectPtr<USceneComponent> SceneRoot;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    TObjectPtr<USphereComponent> CollisionSphere;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    int32 MaxHitTargets = 5; // 최대 공격 가능한 적의 수

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    int32 AbilityLevel = 1; // 스킬 레벨 정보 (이펙트 계산에 활용)

private:
    FGameplayEffectSpecHandle DamageEffectSpecHandle; // 데미지 이펙트 스펙핸들 (AOE 액터가 적용할 데미지 정보)
    FGameplayEffectSpecHandle BuffDebuffEffectSpecHandle; // 버프/디버프 이펙트 스펙핸들 (AOE 액터가 적용할 버프/디버프 정보)
    TArray<FGameplayEffectSpecHandle> StatusEffectSpecHandles; // 상태형 버프 이펙트 스펙핸들 배열 (AOE 액터가 적용할 상태형 버프 정보)

    EAOETargetPolicy TargetPolicy = EAOETargetPolicy::HostileOnly; // 타겟 선정 정책
    TWeakObjectPtr<AActor> InstigatorActor; // 이펙트 적용 시, 원인 제공자 정보로 사용할 액터
};
