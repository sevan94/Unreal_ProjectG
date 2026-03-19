#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayEffectTypes.h"
#include "PGMageMagicBase.generated.h"

class UBoxComponent;
class UNiagaraComponent;

UCLASS()
class UNREAL_PROJECTG_API APGMageMagicBase : public AActor
{
    GENERATED_BODY()

public:
    APGMageMagicBase();

    void SetMagicDamageEffectSpecHandle(const FGameplayEffectSpecHandle& InSpecHandle);

protected:
    virtual void BeginPlay() override;

    UFUNCTION()
    virtual void OnMagicBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    virtual void OnNiagaraFinished(UNiagaraComponent* PSystem);

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Magic|Components")
    TObjectPtr<USceneComponent> RootSceneComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Magic|Components")
    TObjectPtr<UBoxComponent> MagicCollisionComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Magic|Components")
    TObjectPtr<UNiagaraComponent> MagicNiagaraComponent;

    UPROPERTY(BlueprintReadOnly, Category = "Magic", meta = (ExposeOnSpawn = "true"))
    FGameplayEffectSpecHandle MagicDamageEffectSpecHandle;
};