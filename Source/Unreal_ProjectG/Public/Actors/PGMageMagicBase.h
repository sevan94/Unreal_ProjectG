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

    void SetMagicDamageEffectSpecHandle(const TArray<FGameplayEffectSpecHandle>& InSpecHandles);

    // --- [오브젝트 풀링 전용 이벤트] ---    
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Pooling")
    void OnActivatedFromPool();
    virtual void OnActivatedFromPool_Implementation();

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Pooling")
    void OnReturnedToPool();
    virtual void OnReturnedToPool_Implementation();
protected:
    virtual void BeginPlay() override;

    UFUNCTION()
    virtual void OnMagicBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    virtual void OnNiagaraFinished(UNiagaraComponent* PSystem);

private:
    // 파괴(Destroy) 대신 풀로 돌려보내는 헬퍼 함수
    void DeactivateAndReturnToPool();

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Magic|Components")
    TObjectPtr<USceneComponent> RootSceneComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Magic|Components")
    TObjectPtr<UBoxComponent> MagicCollisionComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Magic|Components")
    TObjectPtr<UNiagaraComponent> MagicNiagaraComponent;

    UPROPERTY(BlueprintReadOnly, Category = "Magic", meta = (ExposeOnSpawn = "true"))
    TArray<FGameplayEffectSpecHandle> MagicDamageEffectSpecHandles;

};