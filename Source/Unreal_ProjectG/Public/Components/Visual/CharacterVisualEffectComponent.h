// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/PawnExtensionComponentBase.h"
#include "CharacterVisualEffectComponent.generated.h"

class UMeshComponent;
class UMaterialInterface;
class UMaterialInstanceDynamic;

USTRUCT(BlueprintType)
struct FCharacterMeshMaterialCache
{
    GENERATED_BODY()

    UPROPERTY()
    TArray<TObjectPtr<UMaterialInstanceDynamic>> DynamicMaterialInstances;

    UPROPERTY()
    bool bInitialized = false;
};

/**
 * 
 */
UCLASS()
class UNREAL_PROJECTG_API UCharacterVisualEffectComponent : public UPawnExtensionComponentBase
{
	GENERATED_BODY()
	
public:
    UCharacterVisualEffectComponent();
    
    virtual void BeginPlay() override;

    // 오너의 메시와 머티리얼을 캐싱
    UFUNCTION(BlueprintCallable)
    void InitializeMaterialCachedIfNeeded();

    UFUNCTION(BlueprintCallable)
    void SetHitReactEnabled(bool bEnabled);

    UFUNCTION(BlueprintCallable)
    void SetAOEHighlightEnabled(bool bEnabled);

    UFUNCTION(BlueprintCallable)
    void ResetVisualEffectState();

private:
    void RefreshVisualState();

private:
    UPROPERTY()
    TMap<TObjectPtr<UMeshComponent>, FCharacterMeshMaterialCache> MeshMaterialCache;

    bool bCacheOnBeginPlay = false;
    bool bHitReactEnabled;
    bool bAOEHighlightEnabled;
};
