// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetActor_GroundTrace.h"
#include "GATargetActor_AOEGroundTrace.generated.h"

class USphereComponent;
/**
 * 
 */
UCLASS()
class UNREAL_PROJECTG_API AGATargetActor_AOEGroundTrace : public AGameplayAbilityTargetActor_GroundTrace
{
	GENERATED_BODY()
	
public:
    AGATargetActor_AOEGroundTrace();
    virtual void StartTargeting(UGameplayAbility* InAbility) override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

protected:
    virtual void Tick(float DeltaSeconds) override;
    
    UFUNCTION()
    void OnSphereOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
    
    UFUNCTION()
    void OnSphereOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

    void OnTouchPressed();
    void OnTouchReleased();

    void OnHighlightActorInAOE(AActor* InActor);
    void OnUnhighlightActorOutAOE(AActor* InActor);
    void ClearAllHighlightedActors();
protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Targeting")
    TObjectPtr<USphereComponent> AOERadiusSphere;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Targeting")
    TObjectPtr<UDecalComponent> AOEDecalComponent;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Targeting", meta = (ExposeOnSpawn = true))
    float AOERadius;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Targeting", meta = (ExposeOnSpawn = true))
    float AOEDecalSize;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Targeting", meta = (ExposeOnSpawn = true))
    FRotator AOEDecalRotation;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Targeting", meta = (ExposeOnSpawn = true))
    TObjectPtr<UMaterialInterface> AOETraceDecalMaterial;

    UPROPERTY()
    TArray<TObjectPtr<AActor>> OverlappedActors;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Targeting", meta = (ExposeOnSpawn = true))
    bool bShowDebugSphere = false;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Targeting", meta = (ExposeOnSpawn = true))
    TObjectPtr<AActor> OwnerActor;

    bool bIsTouching = false;
    FVector LastTouchLocation;

    TMap<TObjectPtr<AActor>, TObjectPtr<UMaterialInstanceDynamic>> HighlightedActorMap;
};
