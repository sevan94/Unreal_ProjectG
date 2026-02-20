// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/PGCharacterAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Character/Hero/HeroCharacter.h"
#include "Character/Unit/UnitCharacter.h"

UPGCharacterAttributeSet::UPGCharacterAttributeSet()
{
    InitMaxHealth(100.0f);
    InitHealth(100.0f);
    InitCost(0.0f);
    InitMaxCost(100.0f);
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

void UPGCharacterAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
    Super::PostGameplayEffectExecute(Data);

    if (Data.EvaluatedData.Attribute == GetHealthAttribute())
    {
        SetHealth(FMath::Clamp(GetHealth(), 0.0f, GetMaxHealth()));

        UE_LOG(LogTemp, Log, TEXT("Health : %f"), GetHealth());

        if (FMath::IsNearlyZero(GetHealth()))
        {
            UE_LOG(LogTemp, Log, TEXT("Dead"));
            AHeroCharacter* HeroCharacter = Cast<AHeroCharacter>(GetOwningAbilitySystemComponent()->GetAvatarActor());
            AUnitCharacter* UnitCharacter = Cast<AUnitCharacter>(GetOwningAbilitySystemComponent()->GetAvatarActor());
            if (HeroCharacter)
            {
                HeroCharacter->OnDie();
            }
            if (UnitCharacter)
            {
                UnitCharacter->OnDie();
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