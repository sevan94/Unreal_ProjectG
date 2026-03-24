// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DataAsset_StartupDataBase.generated.h"

class UGameplayEffect;
class UPGGameplayAbility;
class UPGAbilitySystemComponent;
/**
 * 
 */
UCLASS()
class UNREAL_PROJECTG_API UDataAsset_StartupDataBase : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
    // 캐릭터의 어빌리티 시스템 컴포넌트에 게임 시작 시 적용할 어빌리티와 게임 플레이 이펙트를 부여하는 함수
    UFUNCTION(BlueprintCallable, Category = "StartupData")
    virtual void GiveToAbilitySystemComponent(UPGAbilitySystemComponent* InASCToGive, int32 InLevel = 1 );

protected:
    // 어빌리티 시스템 컴포넌트에 게임 시작 시 적용할 어빌리티들을 부여하는 헬퍼 함수
    void GrantAbilities(const TArray<TSubclassOf<UPGGameplayAbility>>& InAbilitiesToGive, UPGAbilitySystemComponent* InASCToGive, int32 ApplyLevel);

protected:
    // 게임 시작 시 캐릭터에 적용할 게임 플레이 어빌리티들(사망, 피격 리액션 등)
    UPROPERTY(EditDefaultsOnly, Category = "StartupData")
    TArray<TSubclassOf<UPGGameplayAbility>> StartUpAbilities;

    // 게임 시작 시 캐릭터에 적용할 게임 플레이 이펙트들(스탯 초기화 등)
    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "StartupData")
    TArray<TSubclassOf<UGameplayEffect>> StartUpGameplayEffects;

};
