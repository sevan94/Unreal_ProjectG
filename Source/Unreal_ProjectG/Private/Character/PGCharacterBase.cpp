// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PGCharacterBase.h"
#include "AbilitySystem/PGAbilitySystemComponent.h"
#include "AbilitySystem/PGCharacterAttributeSet.h"

APGCharacterBase::APGCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;
    PrimaryActorTick.bStartWithTickEnabled = false;

    GetMesh()->SetReceivesDecals(false);

    PGAbilitySystemComponent = CreateDefaultSubobject<UPGAbilitySystemComponent>(TEXT("PGAbilitySystemComponent"));
    CharacterAttributeSet = CreateDefaultSubobject<UPGCharacterAttributeSet>(TEXT("CharacterAttributeSet"));
}

UAbilitySystemComponent* APGCharacterBase::GetAbilitySystemComponent() const
{
    // 그냥 어빌리티 시스템 대신 추가 기능이 있는 PGAbilitySystemComponent 반환
    return GetPGAbilitySystemComponent();
}

UPawnCombatComponent* APGCharacterBase::GetPawnCombatComponent() const
{
    return nullptr;
}

void APGCharacterBase::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const
{
    if(TeamTag.IsValid())
    {
        TagContainer.AddTag(TeamTag);
    }
}

void APGCharacterBase::PossessedBy(AController* NewController)
{
    Super::PossessedBy(NewController);

    if (PGAbilitySystemComponent)
    {
        PGAbilitySystemComponent->InitAbilityActorInfo(this, this);
    }

    // CharacterStartupData가 없으면 경고 로그 출력
    if (CharacterStartupData.IsNull())
    {
        UE_LOG(LogTemp, Warning, TEXT("CharacterStartupData is Null in %s"), *GetName());
    }
}
