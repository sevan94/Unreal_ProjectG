// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/PGUnitGameplayAbility.h"
#include "UnitAbility_Supporter.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_PROJECTG_API UUnitAbility_Supporter : public UPGUnitGameplayAbility
{
	GENERATED_BODY()
public:
    UUnitAbility_Supporter();

    // DataAsset(Config)을 사용하면 OnGiveAbility에서 변수들을 초기화 가능
    virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
    virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
    virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:
    // 애니메이션 노티파이에서 호출될 실제 장판 생성 및 힐/버프 적용 함수
    UFUNCTION(BlueprintCallable, Category = "Ability|Support")
    void HandleSupportEffect(FGameplayEventData InEventData);

    UFUNCTION()
    void OnMontageFinished();

protected:
    // 장판 반경 내 아군에게 부여할 힐/버프 이펙트 (GE)
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability|Support")
    TSubclassOf<UGameplayEffect> SupportEffectClass;

    // 시전 애니메이션
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability|Support")
    TObjectPtr<UAnimMontage> SupportMontage;

    // 힐/버프 계수
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability|Support")
    FScalableFloat SupportSkillMultiplier;

    // 지원 유닛 주위에 깔릴 장판의 탐색 반경
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability|Support")
    float SupportRadius = 500.f;

    // 시각적으로 바닥에 깔아둘 장판 액터 클래스 (데칼이나 파티클을 가진 액터)
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability|Support")
    TSubclassOf<AActor> JangpanActorClass;

    // 장판이 유닛을 따라다닐지(오라 형태), 시전 위치에 남을지 결정
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability|Support")
    bool bAttachToUnit = false;
};
