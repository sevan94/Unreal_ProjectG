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

// 히트 또는 오버랩 시 데미지 적용 정책
UENUM(BlueprintType)
enum class EProjectileDamagePolicy : uint8
{
    OnHit,
    OnBeginOverlap
};

UCLASS()
class UNREAL_PROJECTG_API APGProjectileBase : public AActor
{
	GENERATED_BODY()
	
public:	
	APGProjectileBase();

    void SetProjectileDamageEffectSpecHandle(const FGameplayEffectSpecHandle& InEffectSpecHandle);

protected:
    virtual void Tick(float DeltaTime) override;

	virtual void BeginPlay() override;

    UFUNCTION()
    virtual void OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

    UFUNCTION()
    virtual void OnProjectileBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
    void HandleApplyProjectileDamage(APawn* InHitPawn, const FGameplayEventData& InPayload);

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

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile")
    float ProjectileSpan = 2.0f;
};
