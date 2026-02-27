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

protected:
    //// --- [1] 기지 요소 ---
    //UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
    //TObjectPtr<class UCapsuleComponent> CapsuleComp;

    //UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base")
    //TObjectPtr<class UStaticMeshComponent> MeshComp;

public:
    UPROPERTY(BlueprintAssignable)
    FOnBaseDestroyed OnBaseDestroyed;

    FOnBaseHpChanged OnBaseHpChanged;
    FOnBaseMaxHpChanged OnBaseMaxHpChanged;

    UPROPERTY(EditDefaultsOnly, Category = "GAS")
    TSubclassOf<class UGameplayEffect> InitStatEffect;

protected:
    //// --- [4] 타워 공격 시스템 ---
    //UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attack")
    //TObjectPtr<class USphereComponent> AttackRangeSphere; // 사거리 감지용 구체

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
    float AttackRate = 1.0f; // 공격 속도 (초당 1회)

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
    TSubclassOf<class UGameplayEffect> AttackDamageEffect; // 적에게 입힐 데미지 GE

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
    FGameplayTagContainer TargetTags; // 공격할 대상의 태그 (예: 아군 기지는 'Team.Enemy' 태그를 공격)

    // 사거리 내에 들어온 타겟 목록
    UPROPERTY()
    TArray<AActor*> TargetList;

    FTimerHandle AttackTimerHandle;

    // 오버랩(감지) 이벤트 함수
    UFUNCTION()
    void OnAttackRangeBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    void OnAttackRangeEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

    // 실제 공격 실행 함수
    void ProcessAttack();

public:
   
    virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

    // 블루프린트에서 투사체를 쏘거나 포탄 이펙트를 띄우기 위한 이벤트
    UFUNCTION(BlueprintImplementableEvent, Category = "Attack")
    void BP_OnBaseAttack(AActor* TargetActor);

    // 파괴 처리
    void DestroyBase();

    // UI 연동 함수
    void CurrentHealthChange(const FOnAttributeChangeData& Data) const;
    void MaxHealthChange(const FOnAttributeChangeData& Data) const;

    // UI 테스트용 함수
    void ChangeHP();
};