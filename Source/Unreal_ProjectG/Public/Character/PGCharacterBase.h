// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "Interfaces/PawnCombatInterface.h"
#include "GameplayTagContainer.h"
#include "Types/PGEnumTypes.h"
#include "GameplayTagAssetInterface.h"
#include "PGCharacterBase.generated.h"
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCharacterDiedDelegate, ABaseCharacter*, DeadCharacter);

class UPGCharacterAttributeSet;
class UPGAbilitySystemComponent;
class UDataAsset_StartupDataBase;


UCLASS()
class UNREAL_PROJECTG_API APGCharacterBase : public ACharacter, public IAbilitySystemInterface, public IPawnCombatInterface, public IGameplayTagAssetInterface
{
	GENERATED_BODY()

public:
	APGCharacterBase();

    // 어빌리티 시스템 인터페이스 구현
    virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

    // 컴뱂 인터페이스 구현
    virtual UPawnCombatComponent* GetPawnCombatComponent() const override;

    // 게임 태그 에셋 인터페이스 구현
    virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override;

    FORCEINLINE UPGAbilitySystemComponent* GetPGAbilitySystemComponent() const { return PGAbilitySystemComponent; }
    FORCEINLINE UPGCharacterAttributeSet* GetPGCharacterAttributeSet() const { return CharacterAttributeSet; }
    FORCEINLINE FGameplayTag GetTeamTag() { return TeamTag; }
protected:
    virtual void PossessedBy(AController* NewController) override;

    virtual void OnDie() {};
protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AbilitySystem")
    TObjectPtr<UPGAbilitySystemComponent> PGAbilitySystemComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AbilitySystem")
    TObjectPtr<UPGCharacterAttributeSet> CharacterAttributeSet;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CharacterData", meta = (AllowPrivateAccess = "true"))
    TSoftObjectPtr<UDataAsset_StartupDataBase> CharacterStartupData;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    ETeamType TeamID; // Player, Ally, Enemy 구분

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    FGameplayTag TeamTag;
};
