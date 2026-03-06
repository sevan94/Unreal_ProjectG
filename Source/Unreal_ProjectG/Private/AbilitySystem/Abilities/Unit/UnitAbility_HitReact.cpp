// Fill out your copyright notice in the Description page of Project Settings.
#include "AbilitySystem/Abilities/Unit/UnitAbility_HitReact.h"
#include "Character/Unit/UnitCharacter.h"
#include "TimerManager.h"

void UUnitAbility_HitReact::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    UMeshComponent* TargetMesh = OwnerMesh.Get();

    // 현재 캐싱된 메시가 없다면 ActorInfo에서 메시를 가져와서 캐싱
    if (!TargetMesh)
    {
        if(AUnitCharacter* Unit = Cast<AUnitCharacter>(ActorInfo->AvatarActor.Get()))
        {
            TargetMesh = Unit->GetMesh();
            OwnerMesh = TargetMesh;
        }
    }

    // 메시가 유효하다면 히트 리액트 효과 적용
    if (TargetMesh)
    {
        ApplyHitReactEffect(TargetMesh);
    }

    FTimerHandle TimerHandle;
    GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
        {
            EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
        },
        HitFXDuration, false);
}

void UUnitAbility_HitReact::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
    RestoreOriginalMaterial(OwnerMesh.Get());

    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UUnitAbility_HitReact::ApplyHitReactEffect(UMeshComponent* InTargetMesh)
{
    if (!InTargetMesh) return;

    int32 MaterialCount = InTargetMesh->GetNumMaterials();

    // 처음 실행되는 경우 머티리얼 저장
    if (CachedMaterialData.DynamicMaterials.Num() == 0)
    {
        for (int32 i = 0; i < MaterialCount; i++)
        {
            // 원본 저장
            CachedMaterialData.OriginalMaterials.Add(InTargetMesh->GetMaterial(i));

            // 다이내믹 머티리얼 생성 및 적용
            UMaterialInstanceDynamic* DynamicMaterial = InTargetMesh->CreateAndSetMaterialInstanceDynamic(i);
            CachedMaterialData.DynamicMaterials.Add(DynamicMaterial);
        }
    }
    else
    {
        // 이미 다이내믹 머티리얼이 존재하는 경우, HitFXSwitch만 업데이트
        for (int32 i = 0; i < MaterialCount; i++)
        {
            if (CachedMaterialData.DynamicMaterials.IsValidIndex(i) && CachedMaterialData.DynamicMaterials[i])
            {
                InTargetMesh->SetMaterial(i, CachedMaterialData.DynamicMaterials[i]);
                CachedMaterialData.DynamicMaterials[i]->SetScalarParameterValue(FName("HitFXSwitch"), 1.f);
            }
        }
    }
    InTargetMesh->SetRenderCustomDepth(false); // 셀 셰이딩 비활성화
}

void UUnitAbility_HitReact::RestoreOriginalMaterial(UMeshComponent* InTargetMesh)
{
    if (!InTargetMesh) return;

    if (FMaterialData* MaterialData = &CachedMaterialData)
    {
        InTargetMesh->SetRenderCustomDepth(true); // 셀 셰이딩 활성화

        // 파라미터 초기화
        for (UMaterialInstanceDynamic* MaterialInstance : MaterialData->DynamicMaterials)
        {
            if (MaterialInstance) MaterialInstance->SetScalarParameterValue(FName("HitFXSwitch"), 0.f);
        }

        // 원본 머티리얼로 복원
        for(int32 i = 0; i < MaterialData->OriginalMaterials.Num(); i++)
        {
            InTargetMesh->SetMaterial(i, MaterialData->OriginalMaterials[i]);
        }
    }
}
