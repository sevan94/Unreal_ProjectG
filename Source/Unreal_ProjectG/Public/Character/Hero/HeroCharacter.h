// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/PGCharacterBase.h"
#include "GameplayEffectTypes.h"
#include "GameplayAbilitySpecHandle.h"
#include "Interfaces/EquipmentsStorageInterface.h"
#include "Interfaces/HeroCombatInterface.h"
#include "Types/PGEnumTypes.h"
#include "HeroCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerDied);
// 스테이터스 변화용 델리게이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHeroHpChanged, float, CurrentHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHeroMaxHPChanged, float, MaxHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHeroCostChanged, float, CurrentCost);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHeroMaxCostChanged, float, MaxCost);

class UHeroCombatComponent;
class USphereComponent;
class UDataAsset_WeaponData;
class UDataAsset_ArmorData;
class UDataAsset_AccessoryData;

UCLASS()
class UNREAL_PROJECTG_API AHeroCharacter : public APGCharacterBase, public IEquipmentsStorageInterface, public IHeroCombatInterface
{
    GENERATED_BODY()

public:
    // Sets default values for this character's properties
    AHeroCharacter();

    UHeroCombatComponent* GetHeroCombatComponent() const override { return HeroCombatComponent; }
    UEquipmentsStorageComponent* GetEquipmentsStorageComponent() const override { return EquipmentsStorageComponent; }

    void ChangeCombatMode(EHeroCombatMode NewMode);

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

    //무기 해제
    UFUNCTION(BlueprintCallable, Category = "Equipment")
    void UnEquipWeapon();
    //방어구 해제
    UFUNCTION(BlueprintCallable, Category = "Equipment")
    void UnEquipArmor();
    //악세 해제
    UFUNCTION(BlueprintCallable, Category = "Equipment")
    void UnEquipAccessory();

    // 코스트 소모
    bool ConsumeCost(float InCost);

    //자동전투 상태로 만듦
    UFUNCTION(BlueprintCallable, Category = "Battle")
    void OnAutoBattle() { bIsAuto = true; };

    //자동전투 종료
    UFUNCTION(BlueprintCallable, Category = "Battle")
    void OffAutoBattle() { bIsAuto = false; };

    // UI 업데이트용 함수
    void BroadCastAttributeSet();

    //리스폰 위치 설정
    UFUNCTION()
    void SetRespawnposition(FVector InPosition) { RespawnPosition = InPosition; }

    FORCEINLINE UStaticMeshComponent* GetWeaponStaticMesh() const { return WeaponStaticMesh; }
protected:
    virtual void BeginPlay() override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    // UI 알림용 함수
    void CurrentHealthChange(const FOnAttributeChangeData& Data) const;
    void MaxHealthChange(const FOnAttributeChangeData& Data) const;
    void CurrentCostChange(const FOnAttributeChangeData& Data) const;
    void MaxCostChange(const FOnAttributeChangeData& Data) const;

private:

    //공격 범위 안에 유닛이 들어옴
    UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    //유닛이 공격 범위를 벗어남
    UFUNCTION()
    void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

    // 기본 공격 실행
    UFUNCTION()
    bool TryExecuteBasicAttack_Implementation() override;
    // 스킬 실행
    UFUNCTION()
    bool TryExecuteActiveSkill_Implementation() override;

    // 공격 범위/속도
    UFUNCTION()
    float GetBasicAttackSpeed_Implementation() const override;
    UFUNCTION()
    float GetBasicAttackRange_Implementation() const override;


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
    //방어구 스태틱 메시 컴포넌트
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
    TObjectPtr<class UStaticMeshComponent> ArmorStaticMesh = nullptr;
    //악세 스태틱 메시 컴포넌트
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
    TObjectPtr<class UStaticMeshComponent> AccessoryStaticMesh = nullptr;

    //움직임 컴포넌트
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
    TObjectPtr<class UCharacterMovementComponent> MovementComponent = nullptr;
    // 아이템 장착 상태 저장용 컴포넌트
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
    TObjectPtr<UEquipmentsStorageComponent> EquipmentsStorageComponent = nullptr;
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

    //게임 시작시 실행할 어빌리티
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability")
    TSubclassOf<class UGameplayAbility> GA_Initialize = nullptr;

    //사망 어빌리티
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability")
    TSubclassOf<UGameplayAbility> GA_Die = nullptr;

    //공격
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability")
    TSubclassOf<UGameplayAbility> GA_Attack = nullptr;

    //스킬
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability")
    TArray<TSubclassOf<UGameplayAbility>> GA_Skill;

    //무기
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Equipment")
    TObjectPtr<UDataAsset_WeaponData> Weapon = nullptr;
    //방어구
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Equipment")
    TObjectPtr<UDataAsset_ArmorData> Armor = nullptr;
    //악세
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Equipment")
    TObjectPtr<UDataAsset_AccessoryData> Accessory = nullptr;

private:
    //ABP
    UPROPERTY()
    TObjectPtr<class UAnimInstance> AnimInstance = nullptr;

    //리소스 컴포넌트
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<class UHeroResourceComponent> ResourceManager = nullptr;

    //자동전투 실행 여부
    bool bIsAuto = false;

    //공격 범위 내 적들
    TArray<AActor*> PotentialTargets;

    //공격 어빌리티 핸들
    FGameplayAbilitySpecHandle AttackHandle;
    //스킬 어빌리티 핸들이 들은 배열
    TArray<FGameplayAbilitySpecHandle> SkillHandle;

    //부활할 위치. 아군 베이스의 위치로 설정
    UPROPERTY()
    FVector RespawnPosition = FVector::ZeroVector;
};
