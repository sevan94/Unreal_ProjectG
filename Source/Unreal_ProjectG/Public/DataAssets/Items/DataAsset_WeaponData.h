// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "AbilitySystem/Abilities/Hero/PGHeroSkillGameplayAbility.h"
#include "Engine/DataAsset.h"
#include "DataAsset_WeaponData.generated.h"

class UPGHeroLinkedAnimLayer;
class UDataAsset_HeroSkillData;
class UPGAbilitySystemComponent;

USTRUCT(BlueprintType)
struct FInputAbilityTag
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TSubclassOf<UPGHeroSkillGameplayAbility> AbilityClass; // 스킬 혹은 기본 공격 어빌리티 클래스
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TSoftObjectPtr<UDataAsset_HeroSkillData> AbilityData; // 어빌리티 데이터 에셋

    void LoadData()
    {
        AbilityData.LoadSynchronous();
    }

    bool IsValid() const
    {
        return AbilityClass != nullptr && AbilityData.IsValid();
    }
};
/**
 * 
 */
UCLASS()
class UNREAL_PROJECTG_API UDataAsset_WeaponData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
    // 무기 데이터 에셋으로부터 단일 어빌리티 부여(핸들 반환)
    UFUNCTION(BlueprintCallable, Category = "Abilities", meta = (ApplyLevel = "1"))
    void GrantWeaponAbilityToAbilityComponent(UPGAbilitySystemComponent* InASCToGive, int32 ApplyLevel, TArray<FGameplayAbilitySpecHandle>& OutWeaponAbilitySpecHandles);

public:
    // 무기 스태틱 메시
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TSoftObjectPtr<UStaticMesh> SoftWeaponMesh;

    // 무기가 장착될 소켓 이름
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FName WeaponSocketName;

    // 무기 애니메이션 레이어
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TSoftClassPtr<UPGHeroLinkedAnimLayer> WeaponAnimLayer;

    // 무기 기본 공격 어빌리티
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FInputAbilityTag BasicAttackAbility; // 어빌리티 클래스 + 데이터 에셋

    // 무기 스킬 어빌리티(1번 슬롯 : 무기 공통 스킬, 2번 슬롯 : 무기 고유 스킬)
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FInputAbilityTag SubSkillAbility; // 어빌리티 클래스 + 데이터 에셋

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FInputAbilityTag MainSkillAbility; // 어빌리티 클래스 + 데이터 에셋

    // 무기 공격력
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float WeaponAttackSpeed;

    // 무기 공격 범위
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float WeaponAttackRange;
};
