#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayEffectTypes.h"
#include "Types/PGEnumTypes.h"
#include "AOESkillActor.generated.h"

class USphereComponent;

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

    void InitializeAOEActor(AActor* InInstigator, const TArray<FGameplayEffectSpecHandle>& InSpecHandles, EAOETargetPolicy InTargetPolicy, int32 InMaxHitTargets);

    UFUNCTION(BlueprintPure)
    float GetAOERadius() const;

    UFUNCTION(BlueprintCallable, meta = (DisplayName = "CollectAndApplyEffects"))
    void BP_CollectAndApplyEffects();

private:
    bool IsValidTarget(AActor* InTarget) const;
    void ApplySpecHandlesToSingleTarget(AActor* InTarget);

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    TObjectPtr<USceneComponent> SceneRoot;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    TObjectPtr<USphereComponent> CollisionSphere;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    bool bApplyOnSpawn; // 스폰 될 떄 바로 이펙트 적용할지 여부

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    bool bIsPersistent; // 지속형 AOE 스킬인지 여부 (true면 CollisionSphere는 계속 활성화, false면 한 번 적용 후 비활성화)

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (EditCondition = "bIsPersistent", ClampMin = "0.0"))
    float Duration; // 지속형 AOE 스킬일 때, 효과가 지속되는 시간

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (EditCondition = "bIsPersistent", ClampMin = "0.1"))
    float TickInterval; // 지속형 AOE 스킬일 때, 효과가 적용되는 간격

private:
    TArray<FGameplayEffectSpecHandle> EffectSpecHandles; // 적용할 이펙트 스펙 핸들 배열
    EAOETargetPolicy TargetPolicy = EAOETargetPolicy::HostileOnly; // 타겟 선정 정책
    int32 MaxHitTargets = 5; // 최대 공격 가능한 적의 수
    TWeakObjectPtr<AActor> InstigatorActor; // 이펙트 적용 시, 원인 제공자 정보로 사용할 액터
};
