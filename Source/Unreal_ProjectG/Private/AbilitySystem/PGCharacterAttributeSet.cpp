// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/PGCharacterAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "Character/PGCharacterBase.h"
#include "Pawn/BaseStructure.h"

UPGCharacterAttributeSet::UPGCharacterAttributeSet()
{
    InitMaxHealth(100.0f);
    InitHealth(GetMaxHealth());
    InitMaxCost(100.0f);
    InitCost(GetMaxCost());
    InitAttackPower(10.0f);
    InitAttackSpeed(1.0f);
}

void UPGCharacterAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
    Super::PreAttributeChange(Attribute, NewValue);

    if (Attribute == GetHealthAttribute())
    {
        NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHealth());
    }
    if (Attribute == GetCostAttribute())
    {
        NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxCost());
    }
}

//bool UPGCharacterAttributeSet::PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data)
//{
//    Super::PreGameplayEffectExecute(Data);
//
//    if (Data.EvaluatedData.Attribute == GetMaxHealthAttribute())
//    {
//        if (ABaseStructure* BaseStructure = Cast<ABaseStructure>(GetOwningAbilitySystemComponent()->GetAvatarActor()))
//        {
//            UE_LOG(LogTemp, Log, TEXT("%s Pre 기지 최대 체력: %f"), *BaseStructure->GetName(), GetMaxHealth());
//            return true;
//
//        }
//    }
//    return false;
//}

void UPGCharacterAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
    Super::PostGameplayEffectExecute(Data);

    APGCharacterBase* BaseCharacter = Cast<APGCharacterBase>(GetOwningAbilitySystemComponent()->GetAvatarActor());

    if (Data.EvaluatedData.Attribute == GetHealthAttribute())
    {
        UE_LOG(LogTemp, Log, TEXT("Health : %f"), GetHealth());
        SetHealth(FMath::Clamp(GetHealth(), 0.0f, GetMaxHealth()));

        if (FMath::IsNearlyZero(GetHealth()))
        {
            UE_LOG(LogTemp, Log, TEXT("Dead"));
            if (BaseCharacter)
            {
                BaseCharacter->OnDie();
            }
        }
    }

    if (Data.EvaluatedData.Attribute == GetCostAttribute())
    {
        SetCost(FMath::Clamp(GetCost(), 0.0f, GetMaxCost()));

        UE_LOG(LogTemp, Log, TEXT("Cost : %f"), GetCost());
    }
}