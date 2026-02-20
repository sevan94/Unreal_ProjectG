// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "Framework/GASMacro.h"
#include "PGCharacterAttributeSet.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_PROJECTG_API UPGCharacterAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
public:
    UPGCharacterAttributeSet();

    virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
    virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

public:
    UPROPERTY(BlueprintReadOnly, Category = "Attribute")
    FGameplayAttributeData Health;
    ATTRIBUTE_ACCESSORS(UPGCharacterAttributeSet, Health)

    UPROPERTY(BlueprintReadOnly, Category = "Attribute")
    FGameplayAttributeData MaxHealth;
    ATTRIBUTE_ACCESSORS(UPGCharacterAttributeSet, MaxHealth)

    UPROPERTY(BlueprintReadOnly, Category = "Attribute")
    FGameplayAttributeData Cost;
    ATTRIBUTE_ACCESSORS(UPGCharacterAttributeSet, Cost)

    UPROPERTY(BlueprintReadOnly, Category = "Attribute")
    FGameplayAttributeData MaxCost;
    ATTRIBUTE_ACCESSORS(UPGCharacterAttributeSet, MaxCost)

    UPROPERTY(BlueprintReadOnly, Category = "Attribute")
    FGameplayAttributeData AttackPower;
    ATTRIBUTE_ACCESSORS(UPGCharacterAttributeSet, AttackPower)

    UPROPERTY(BlueprintReadOnly, Category = "Attribute")
    FGameplayAttributeData AttackSpeed;
    ATTRIBUTE_ACCESSORS(UPGCharacterAttributeSet, AttackSpeed)

    UPROPERTY(BlueprintReadOnly, Category = "Attribute")
    FGameplayAttributeData MoveSpeed;
    ATTRIBUTE_ACCESSORS(UPGCharacterAttributeSet, MoveSpeed)
};
