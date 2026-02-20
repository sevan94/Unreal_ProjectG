// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/PGCharacterBase.h"
#include "GameplayEffectTypes.h"
#include "HeroCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerDied);
// 스테이터스 변화용 델리게이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHeroHpChanged, float, CurrentHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHeroMaxHPChanged, float, MaxHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHeroCostChanged, float, CurrentCost);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHeroMaxCostChanged, float, MaxCost);

class UHeroCombatComponent;
class USphereComponent;

UCLASS()
class UNREAL_PROJECTG_API AHeroCharacter : public APGCharacterBase
{
    GENERATED_BODY()

public:
    // Sets default values for this character's properties
    AHeroCharacter();

    virtual UPawnCombatComponent* GetPawnCombatComponent() const override;

    //캐릭터 스폰(시작 혹은 부활 시)
    UFUNCTION(BlueprintCallable, Category = "HeroCharacter")
    void SpawnHero();

    //사망 호출
    UFUNCTION(BlueprintCallable, Category = "HeroCharacter")
    virtual void OnDie() override;

    //사망상태로 만듬
    UFUNCTION()
    void MakeHeroDead();

    //게임 시작시 호출할 함수. 코스트 재생 등
    UFUNCTION(BlueprintCallable, Category = "HeroCharacter")
    void InitializeHero();

    // UI 업데이트용 함수
    void BroadCastAttributeSet();

    FORCEINLINE UHeroCombatComponent* GetHeroCombatComponent() const { return HeroCombatComponent; }
    FORCEINLINE UStaticMeshComponent* GetWeaponStaticMesh() const { return WeaponStaticMesh; }
protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    // UI 알림용 함수
    void CurrentHealthChange(const FOnAttributeChangeData& Data) const;
    void MaxHealthChange(const FOnAttributeChangeData& Data) const;
    void CurrentCostChange(const FOnAttributeChangeData& Data) const;
    void MaxCostChange(const FOnAttributeChangeData& Data) const;

private:
    //이동
    UFUNCTION()
    void OnMovementInput(const FInputActionValue& InValue);
    //공격
    UFUNCTION()
    void OnAttackInput();

    UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

    UFUNCTION()
    void ActivateAttack();

    UFUNCTION()
    AActor* GetClosestTarget(const TArray<AActor*>& TargetArray);

public:
    UPROPERTY(BlueprintAssignable, Category = "Event")
    FOnPlayerDied OnPlayerDied;

    FOnHeroHpChanged OnHeroHpChanged;
    FOnHeroMaxHPChanged OnHeroMaxHpChanged;
    FOnHeroCostChanged OnHeroCostChanged;
    FOnHeroMaxCostChanged OnHeroMaxCostChanged;

protected:
    //컴포넌트
    //스프링 암
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
    TObjectPtr<class USpringArmComponent> SpringArm = nullptr;
    //카메라
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
    TObjectPtr<class UCameraComponent> CameraComponent = nullptr;
    //무기 스태틱 메시 컴포넌트
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
    TObjectPtr<class UStaticMeshComponent> WeaponStaticMesh = nullptr;

    //움직임 컴포넌트
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
    TObjectPtr<class UCharacterMovementComponent> MovementComponent = nullptr;
    // 컴뱃 컴포넌트
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
    TObjectPtr<UHeroCombatComponent> HeroCombatComponent = nullptr;
    //공격 판정용 콜리전. 공격 범위 처리용
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Component")
    TObjectPtr<class USphereComponent> AggroCollision = nullptr;

    //input action
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "InputAction")
    TObjectPtr<class UInputAction> IA_Move = nullptr;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "InputAction")
    TObjectPtr<UInputAction> IA_Attack = nullptr;

    //리소스를 관리하는 어트리뷰트
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<class UPGCharacterAttributeSet> ResourceAttribute = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability")
    TSubclassOf<class UGameplayAbility> GA_Initialize = nullptr;

    //사망 어빌리티
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability")
    TSubclassOf<UGameplayAbility> GA_Die = nullptr;

    //공격
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability")
    TSubclassOf<UGameplayAbility> GA_Attack = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Equipment")
    TObjectPtr<class UDataAsset_WeaponData> Weapon = nullptr;

private:
    //ABP
    UPROPERTY()
    TObjectPtr<class UAnimInstance> AnimInstance = nullptr;

    //리소스 컴포넌트
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<class UHeroResourceComponent> ResourceManager = nullptr;

    TArray<AActor*> PotentialTargets;
};
