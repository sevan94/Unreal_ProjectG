// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataAssets/StartUp/DataAsset_StartupDataBase.h"
#include "GameplayTagContainer.h"
#include "DataAsset_UnitStartupData.generated.h"
class UBranchDataAsset;
class UPGUnitGameplayAbility;

/**
 * 
 */
UCLASS()
class UNREAL_PROJECTG_API UDataAsset_UnitStartupData : public UDataAsset_StartupDataBase
{
	GENERATED_BODY()
	
public:
    virtual void GiveToAbilitySystemComponent(UPGAbilitySystemComponent* InASCToGive, int32 InLevel = 1) override;

    // --- [1] 기본 정보 ---
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Info")
    FGameplayTag ElementTag;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Info")
    FGameplayTag TeamTag;

    UPROPERTY(EditAnywhere, Category = "Stats")
    UBranchDataAsset* BranchData;

    // --- [2] 외형 ---
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Visual")
    USkeletalMesh* SkeletalMesh;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Visual")
    TSubclassOf<UAnimInstance> AnimBlueprint;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Visual")
    UAnimMontage* UnitBasicAttackMontage;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Visual")
    UAnimMontage* UnitSkillMontage;

    //// --- [3] 능력치 (GAS 초기화용) ---
    //UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stats")
    //float Health = 100.0f;

    //UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stats")
    //float AttackDamage = 10.0f;

    //UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stats")
    //float AttackSpeed = 1.0f;

    //UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stats")
    //float MoveSpeed = 300.0f;
private:
    UPROPERTY(EditDefaultsOnly, Category = "StartupData")
    TArray<TSubclassOf<UPGUnitGameplayAbility>> UnitCombatAbilities;
};
