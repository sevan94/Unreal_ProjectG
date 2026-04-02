// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayEffectTypes.h"
#include "PGProjectileBase.generated.h"

class UBoxComponent;
class UNiagaraComponent;
class UProjectileMovementComponent;
struct FGameplayEventData;
class UNiagaraSystem;
class USoundBase;

// 히트 또는 오버랩 시 데미지 적용 정책
UENUM(BlueprintType)
enum class EProjectileDamagePolicy : uint8
{
    OnHit,
    OnBeginOverlap
};

UCLASS(Blueprintable)
class UNREAL_PROJECTG_API APGProjectileBase : public AActor
{
	GENERATED_BODY()
	
public:	
	APGProjectileBase();

    void SetProjectileDamageEffectSpecHandle(const FGameplayEffectSpecHandle& InEffectSpecHandle);

    // --- [오브젝트 풀링 전용 이벤트] ---   
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Pooling")
    void OnActivatedFromPool();

    virtual void OnActivatedFromPool_Implementation();

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Pooling")
    void OnReturnedToPool();

    virtual void OnReturnedToPool_Implementation();
protected:
    virtual void Tick(float DeltaTime) override;

	virtual void BeginPlay() override;

    UFUNCTION()
    virtual void OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

    UFUNCTION()
    virtual void OnProjectileBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
    void HandleApplyProjectileDamage(APawn* InHitPawn, const FGameplayEventData& InPayload);

    // 파괴(Destroy) 대신 풀로 돌려보내는 도우미 함수
    void DeactivateAndReturnToPool();
protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile|Components")
    TObjectPtr<UBoxComponent> ProjectileCollisionComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile|Components")
    TObjectPtr<UNiagaraComponent> ProjectileNiagaraComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile|Components")
    TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile")
    EProjectileDamagePolicy ProjectileDamagePolicy = EProjectileDamagePolicy::OnBeginOverlap;

    UPROPERTY(BlueprintReadOnly, Category = "Projectile", meta = (ExposeOnSpawn = "true"))
    FGameplayEffectSpecHandle ProjectileDamageEffectSpecHandle;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile|SVFX")
    TWeakObjectPtr<UNiagaraSystem> ProjectileImpactVFX;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile|SVFX")
    TWeakObjectPtr<UNiagaraSystem> HitImpactVFX;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile|SVFX")
    TWeakObjectPtr<USoundBase> ProjectileImpactSFX;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile")
    float ProjectileSpan = 2.0f;

private:
    // 수명(LifeSpan)을 직접 관리하기 위한 타이머
    FTimerHandle LifeSpanTimerHandle;
};
