// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayEffectTypes.h"
#include "ScalableFloat.h"
#include "DataAsset_ArmorData.generated.h"

class UGameplayEffect;
class UPGAbilitySystemComponent;

USTRUCT(BlueprintType)
struct FArmorModifierEntry
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FGameplayAttribute Attribute;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FScalableFloat ModifierValue;
};
/**
 * 
 */
UCLASS()
class UNREAL_PROJECTG_API UDataAsset_ArmorData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
    UFUNCTION(BlueprintCallable, Category = "Armor|Data")
    void MakeOutgoingArmorEffectSpecHandle(UPGAbilitySystemComponent* InASC, int32 InLevel = 1) const;

public:
    // 세트 구분을 위한 태그
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (Categories = "Set"))
    FGameplayTag SetTag;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TArray<FArmorModifierEntry> AttributeModifiers;
};
