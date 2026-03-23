#pragma once

#include "CoreMinimal.h"
#include "Character/PGCharacterBase.h"
#include "AbilitySystemInterface.h" 
#include "Components/SphereComponent.h"
#include "GameplayTagContainer.h"
#include "Types/PGEnumTypes.h"
#include "GameplayEffectTypes.h"
#include "BaseStructure.generated.h"

// 델리게이트 선언 (기지 파괴 알림용)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBaseDestroyed, ETeamType, DestroyedTeam);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnBaseHpChanged, FGameplayTag, TeamTag, float, CurrentHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnBaseMaxHpChanged, FGameplayTag, TeamTag, float, MaxHealth);


UCLASS()
class UNREAL_PROJECTG_API ABaseStructure : public APGCharacterBase
{
    GENERATED_BODY()

public:
    ABaseStructure();

protected:
    virtual void BeginPlay() override;

public:
    UPROPERTY(BlueprintAssignable)
    FOnBaseDestroyed OnBaseDestroyed;

    FOnBaseHpChanged OnBaseHpChanged;
    FOnBaseMaxHpChanged OnBaseMaxHpChanged;

    UPROPERTY(EditDefaultsOnly, Category = "GAS")
    TSubclassOf<class UGameplayEffect> InitStatEffect;

public:
    virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

    virtual void OnDie() override;

    // 파괴 처리
    void DestroyBase();

    // UI 연동 함수
    void CurrentHealthChange(const FOnAttributeChangeData& Data) const;
    void MaxHealthChange(const FOnAttributeChangeData& Data) const;
protected:
    UFUNCTION()
    void ActivateInvincible(FGameplayTag InTeamTag);

    UFUNCTION()
    void DeActivateInvincible(FGameplayTag InTeamTag);

    UFUNCTION(BlueprintImplementableEvent, Category = "Base|Effect")
    void BP_OnToggleInvincibleEffect(bool bIsInvincible);
};