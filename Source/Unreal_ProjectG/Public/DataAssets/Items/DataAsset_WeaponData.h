// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
//#include "Types/PGStructTypes.h"
#include "DataAsset_WeaponData.generated.h"

class UPGHeroGameplayAbility;
class UPGHeroLinkedAnimLayer;
class UPGGameplayAbility;
class UDataAsset_SkillData;

/**
 * 
 */
UCLASS()
class UNREAL_PROJECTG_API UDataAsset_WeaponData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
//public:
    //FORCEINLINE const FPGHeroWeaponData& GetHeroWeaponData() const { return HeroWeaponData; }

public:
    // 세트 구분을 위한 태그
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (Categories = "Set"))
    FGameplayTag SetTag;

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
    TSoftObjectPtr<UDataAsset_SkillData> WeaponBasicAttackData;

    // 무기 스킬 어빌리티(1번 슬롯 : 무기 공통 스킬, 2번 슬롯 : 무기 고유 스킬)
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<TSoftObjectPtr<UDataAsset_SkillData>> WeaponSkillDataArray;

    // 무기 공격력
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float WeaponAttackSpeed;

    // 무기 공격 범위
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float WeaponAttackRange;
};
