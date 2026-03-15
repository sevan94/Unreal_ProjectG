// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Visual/CharacterVisualEffectComponent.h"

#include "Components/MeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"

UCharacterVisualEffectComponent::UCharacterVisualEffectComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UCharacterVisualEffectComponent::BeginPlay()
{
    Super::BeginPlay();

    if (bCacheOnBeginPlay)
    {
        InitializeMaterialCachedIfNeeded();
        RefreshVisualState();
    }
}

void UCharacterVisualEffectComponent::InitializeMaterialCachedIfNeeded()
{
    // 이미 캐시가 존재한다면 초기화할 필요 없음
    if(MeshMaterialCache.Num() > 0) return;

    AActor* OwnerActor = GetOwner();
    if(!OwnerActor) return;

    TArray<UMeshComponent*> MeshComponents;
    OwnerActor->GetComponents<UMeshComponent>(MeshComponents);

    // 모든 메시 컴포넌트에 대해 원본 머티리얼과 동적 머티리얼 인스턴스를 생성하여 캐시에 저장
    for (UMeshComponent* MeshComponent : MeshComponents)
    {
        if (!IsValid(MeshComponent)) continue;

        const int32 MaterialCount = MeshComponent->GetNumMaterials();
        if (MaterialCount <= 0) continue;

        FCharacterMeshMaterialCache MaterialCache;
        MaterialCache.DynamicMaterialInstances.Reserve(MaterialCount);

        // 각 머티리얼 슬롯에 대해 동적 머티리얼 인스턴스를 생성하여 캐시에 저장
        for(int32 MaterialIndex = 0; MaterialIndex < MaterialCount; MaterialIndex++)
        {
            UMaterialInstanceDynamic* DynamicMaterialInstance = MeshComponent->CreateAndSetMaterialInstanceDynamic(MaterialIndex);
            MaterialCache.DynamicMaterialInstances.Add(DynamicMaterialInstance);
        }

        MaterialCache.bInitialized = true;
        MeshMaterialCache.Add(MeshComponent, MoveTemp(MaterialCache)); // 복사가 아닌 메모리 이동을 통해 캐시에 저장(성능 최적화)
    }
}

void UCharacterVisualEffectComponent::SetHitReactEnabled(bool bEnabled)
{
    if (bHitReactEnabled == bEnabled) return;

    bHitReactEnabled = bEnabled;
    RefreshVisualState();
}

void UCharacterVisualEffectComponent::SetAOEHighlightEnabled(bool bEnabled)
{
    if (bAOEHighlightEnabled == bEnabled) return;

    bAOEHighlightEnabled = bEnabled;
    RefreshVisualState();
}

void UCharacterVisualEffectComponent::ResetVisualEffectState()
{
    bHitReactEnabled = false;
    bAOEHighlightEnabled = false;
    RefreshVisualState();
}

void UCharacterVisualEffectComponent::RefreshVisualState()
{
    InitializeMaterialCachedIfNeeded();
    
    // 히트 리액트 또는 AOE 하이라이트가 활성화된 경우 셀 셰이딩 효과를 비활성화
    const bool bDisableCelShading = bHitReactEnabled || bAOEHighlightEnabled;

    for (TPair<TObjectPtr<UMeshComponent>, FCharacterMeshMaterialCache>& Pair : MeshMaterialCache)
    {
        UMeshComponent* MeshComponent = Pair.Key;
        FCharacterMeshMaterialCache& MaterialCache = Pair.Value;

        // 메시 컴포넌트 또는 머티리얼 캐시가 유효하지 않다면 해당 컴포넌트는 건너뜀
        if (!IsValid(MeshComponent) || !MaterialCache.bInitialized) continue;

        MeshComponent->SetRenderCustomDepth(bDisableCelShading);

        for(UMaterialInstanceDynamic* DynamicMaterialInstance : MaterialCache.DynamicMaterialInstances)
        {
            if (!IsValid(DynamicMaterialInstance)) continue;
            
            // HitReact와 AOEHighlight의 활성화 상태 합성
            DynamicMaterialInstance->SetScalarParameterValue(TEXT("HitFXSwitch"), bHitReactEnabled ? 1.0f : 0.0f);
            DynamicMaterialInstance->SetScalarParameterValue(TEXT("OverlapFXSwitch"), bAOEHighlightEnabled ? 1.0f : 0.0f);
        }
    }
}
