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
    UFUNCTION(BlueprintCallable, Category = "StartupData")
    virtual void GiveToAbilitySystemComponent(UPGAbilitySystemComponent* InASCToGive, int32 InLevel = 1 );

protected:
    // 게임 시작 시 캐릭터에 적용할 게임 플레이 이펙트들(스탯 초기화 등)
    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "StartupData")
    TArray<TSubclassOf<UGameplayEffect>> StartUpGameplayEffects;

};
