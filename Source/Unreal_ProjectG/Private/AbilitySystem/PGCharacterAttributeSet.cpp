// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/PGCharacterAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Character/PGCharacterBase.h"

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

void UPGCharacterAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
    Super::PostAttributeChange(Attribute, OldValue, NewValue);

    if (Attribute == GetMaxHealthAttribute())
    {
        const float Delta = NewValue - OldValue;

        if (Delta > 0.f)
        {
            // MaxHealth가 증가 했으면 CurrentHealth도 같이 증가 시킨다.
            const float NewHealth = FMath::Min(GetHealth() + Delta, NewValue);
            SetHealth(NewHealth);
        }
        else
        {
            // MaxHealth가 감소 했으면 CurrentHealth는 Clamp만 하여 MaxHealth보다 높지 않도록 한다.
            if (GetHealth() > NewValue)
            {
                SetHealth(NewValue);
            }
        }
    }
}

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

    if(Data.EvaluatedData.Attribute == GetMoveSpeedAttribute())
    {
        SetMoveSpeed(FMath::Max(0.0f, GetMoveSpeed()));

        ACharacter* Character = Cast<ACharacter>(GetOwningAbilitySystemComponent()->GetAvatarActor());
        if(Character)
        {
            Character->GetCharacterMovement()->MaxWalkSpeed = GetMoveSpeed();
        }
    }
}