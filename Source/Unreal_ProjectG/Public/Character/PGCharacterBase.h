// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "Types/PGEnumTypes.h"
#include "GameplayTagAssetInterface.h"
#include "PGCharacterBase.generated.h"

//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCharacterDiedDelegate, ABaseCharacter*, DeadCharacter);

class UPGCharacterAttributeSet;
class UPGAbilitySystemComponent;
class UDataAsset_StartupDataBase;


UCLASS()
class UNREAL_PROJECTG_API APGCharacterBase : public ACharacter, public IAbilitySystemInterface, public IGameplayTagAssetInterface
{
	GENERATED_BODY()

public:
	APGCharacterBase();

    // 어빌리티 시스템 인터페이스 구현
    virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

    // 게임 태그 에셋 인터페이스 구현
    virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override;

    virtual void OnDie() {};

    FORCEINLINE FGameplayTag GetTeamTag() { return TeamTag; }
    FORCEINLINE UPGAbilitySystemComponent* GetPGAbilitySystemComponent() const { return PGAbilitySystemComponent; }
    FORCEINLINE UPGCharacterAttributeSet* GetPGCharacterAttributeSet() const { return CharacterAttributeSet; }

    UFUNCTION(BlueprintCallable)
    FORCEINLINE FGameplayTag GetBranchTag(){ return BranchTag; }

    UFUNCTION(BlueprintCallable)
    FORCEINLINE FGameplayTag GetElementTag() { return ElementTag; }
protected:
    virtual void PossessedBy(AController* NewController) override;

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AbilitySystem")
    TObjectPtr<UPGAbilitySystemComponent> PGAbilitySystemComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AbilitySystem")
    TObjectPtr<UPGCharacterAttributeSet> CharacterAttributeSet;

    // 캐릭터 초기화 데이터 에셋
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CharacterData", meta = (AllowPrivateAccess = "true"))
    TSoftObjectPtr<UDataAsset_StartupDataBase> CharacterStartupData;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    ETeamType TeamID; // Player, Ally, Enemy 구분

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    FGameplayTag TeamTag;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    FGameplayTag BranchTag;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    FGameplayTag ElementTag;
};






