// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/PGUnitGameplayAbility.h"
#include "UnitAbility_HitReact.generated.h"
/*셀 세이딩 켜져있으면 제대로 빛이 안나옴, 범위 안에 있으면 셀 세이딩 off, 다시 나가면 on 하는 기능 추가해야 함.
* 해당 액터의 메시를 가져와서 메시->SetRenderCustomDepth(ture(셀 세이딩 켜짐)/false(셀 세이딩 꺼짐)); 쓰면 됨
*/

/**
 * 
 */
UCLASS()
class UNREAL_PROJECTG_API UUnitAbility_HitReact : public UPGUnitGameplayAbility
{
	GENERATED_BODY()
	
public:
    virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
    virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HitReact")
    float HitFXDuration = 0.2f;

private:
    UPROPERTY()
    TWeakObjectPtr<UMeshComponent> OwnerMesh;
};
