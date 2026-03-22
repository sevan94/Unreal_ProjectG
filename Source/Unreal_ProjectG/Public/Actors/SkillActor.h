#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayEffectTypes.h"
#include "DataAssets/Ability/DataAsset_HeroSkillData.h"
#include "SkillActor.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UDataAsset_SkillVisualData;

/**
* 스킬 액터(투사체 / 장판 / 폭발 등등)
* 1. SpawnActorDeferred->InitFromConfig를 통하여 스킬 액터가 초기화
* BeginPlay에서 타이머/콜리전을 설정하므로 InitFromConfig가 반드시 먼저 호출되어야 함
* 
* 동작 방식은 Config를 통하여 결정
* Speed > 0 : 투사체
* TickInterval > 0 : 다단히트/장판
* TickInterval == 0 충돌 즉시 이펙트 적용
* 
* 파괴 시 Owner에게 Shared.Event.SkillActorDestroyed 태그 전달
* GA의 WaitEvent가 이 이벤트를 받아 다음 액션을 실행
*/

UCLASS()
class UNREAL_PROJECTG_API ASkillActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ASkillActor();

    // Beginplay보다 먼저 호출을 보장 받아야 함
    void InitFromConfig(const FHeroSpawnableConfig& InConfig, const TArray<FGameplayEffectSpecHandle>& InSpecHandles);

    // Task가 파괴 이벤트 수신에 사욯할 태그
    FGameplayTag GetDestroyedEventTag() const;

protected:
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    //==================================================
    // BP에서 사용할 이벤트
    //==================================================
    // 틱마다 호출
    UFUNCTION(BlueprintImplementableEvent, Category = "SkillActor|Events", meta = (DisplayName = "On Tick Effect For Target"))
    void OnTickEffectForTarget(AActor* TargetActor);

    // 즉발 충돌 시 호출
    UFUNCTION(BlueprintImplementableEvent, Category = "SkillActor|Events", meta = (DisplayName = "On Instant Hit"))
    void OnInstantHit(AActor* TargetActor);

    // 파괴 직전 호출
    UFUNCTION(BlueprintImplementableEvent, Category = "SkillActor|Events", meta = (DisplayName = "On Skill Actor Destroyed"))
    void OnSkillActorDestroyed();

    //==================================================
    // BP에서 호출 가능한 함수
    // ==================================================
    // 특정 타겟에 GE적용
    UFUNCTION(BlueprintCallable, Category = "SkillActor")
    void ApplyEffectsToTarget(AActor* TargetActor);

    // 파괴 트리거
    UFUNCTION(BlueprintCallable, Category = "SkillActor")
    void NotifyAndDestroy();

    // VFX 트리거
    UFUNCTION(BlueprintCallable, Category = "SkillActor")
    void PlayImpactVFX(const FVector& Location);

private:
    // 충돌 이벤트 콜백
    UFUNCTION()
    void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
    
    UFUNCTION()
    void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

    //==================================================
    // 게임 플레이 이펙트 적용 함수
    //==================================================
    
    // TickInterval > 0; 타이머 콜백
    void HandleTickEffects();

    // 단발성, 충돌 시 바로 적용
    void HandleInstantEffects(AActor* TargetActor);

    // TargetPolicy에 따라 타겟이 유효한지 검사
    bool IsValidTarget(AActor* TargetActor) const;

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SkillActor|Components")
    TObjectPtr<USceneComponent> SceneRoot;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SkillActor|Components")
    TObjectPtr<USphereComponent> CollisionComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SkillActor|Components")
    TObjectPtr<UProjectileMovementComponent> ProjectileMovement;

    // 날아가는 동안 재생할 프로젝타일 이펙트 컴포넌트 (없을 수도 있음)
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SkillActor|Components")
    TObjectPtr<UNiagaraComponent> ActorVFXComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SkillActor|Components")
    TObjectPtr<UAudioComponent> ActorSFXComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillActor|Visual")
    TObjectPtr<UDataAsset_SkillVisualData> VisualAsset;

    UPROPERTY(BlueprintReadOnly, Category = "SkillActor")
    FHeroSpawnableConfig Config;

    UPROPERTY(BlueprintReadOnly, Category = "SkillActor")
    TArray<FGameplayEffectSpecHandle> EffectSpecHandles;

    UPROPERTY(BlueprintReadOnly, Category = "SkillActor")
    TArray<TObjectPtr<AActor>> OverlappingTargets;

    FTimerHandle TickEffectTimerHandle;

    TMap <TWeakObjectPtr<AActor>, float> HitCooldownMap; // TickInterval > 0인 경우, 타겟별로 히트 쿨타임 관리

private:
    bool bDestroyNotified = false; // 파괴 통보 여부 (중복 방지)
    static const FGameplayTag DestroyedEventTag; // 파괴 이벤트 태그
};
