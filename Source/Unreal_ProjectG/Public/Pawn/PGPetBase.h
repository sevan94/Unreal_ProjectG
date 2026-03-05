// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GameplayTagContainer.h"
#include "AbilitySystemInterface.h"
#include "PGPetBase.generated.h"

class UFloatingPawnMovement;
class AHeroCharacter;
class UPGAbilitySystemComponent;

class UDataAsset_SkillData;

UCLASS()
class UNREAL_PROJECTG_API APGPetBase : public APawn, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	APGPetBase();

protected:
	virtual void BeginPlay() override;
    
public:
    virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    TObjectPtr<USkeletalMeshComponent> PetSkeletalMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    TObjectPtr<UFloatingPawnMovement> PetMovementComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    TObjectPtr<UPGAbilitySystemComponent> PGAbilitySystemComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    TWeakObjectPtr<AHeroCharacter> OwnerHero;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FGameplayTag TeamTag;

    // ================================================
    // 데이터 에셋으로 분리 고려
    // ================================================
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TArray<TObjectPtr<UDataAsset_SkillData>> SkillDataAssets;
};
