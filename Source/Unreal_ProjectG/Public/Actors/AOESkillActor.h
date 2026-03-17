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

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FScalableFloat SkillMultiplier;
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FScalableFloat BaseBuffAmount;
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float Duration;
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

    void InitializeAOEActor(AActor* InInstigator, EAOETargetPolicy InTargetPolicy, FGameplayEffectSpecHandle& InEffectSpecHandle,int32 InAbilityLevel=1);

    UFUNCTION(BlueprintPure)
    float GetAOERadius() const;

    UFUNCTION(BlueprintCallable)
    TArray<AActor*> CollectValidTargets();

    UFUNCTION(BlueprintPure)
    const TArray<AActor*>& GetOverlappedTargets() const { return OverlappedTargets; }

protected:
    UFUNCTION(BlueprintPure)
    float GetScalableValueAtLevel(const FScalableFloat& InScalableFloat) const;

    bool IsValidTarget(AActor* InTarget) const;

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    TObjectPtr<USceneComponent> SceneRoot;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    TObjectPtr<USphereComponent> CollisionSphere;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    int32 MaxHitTargets = 5; // 최대 공격 가능한 적의 수

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    int32 AbilityLevel = 1; // 스킬 레벨 정보 (이펙트 계산에 활용)

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    TWeakObjectPtr<AActor> InstigatorActor; // 이펙트 적용 시, 원인 제공자 정보로 사용할 액터

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    EAOETargetPolicy TargetPolicy = EAOETargetPolicy::HostileOnly; // 타겟 선정 정책

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    FGameplayEffectSpecHandle EffectSpecHandle; // 적용할 이펙트 스펙 핸들

private:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    TArray<TObjectPtr<AActor>> OverlappedTargets; // AOE 범위 내에 겹쳐진 액터들 (디버깅 및 추가 로직 활용을 위해 저장)
};
