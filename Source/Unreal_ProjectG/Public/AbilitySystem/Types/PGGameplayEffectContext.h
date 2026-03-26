// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "GameplayTagContainer.h"
#include "GameplayEffectTypes.h"
#include "Types/PGEnumTypes.h"
#include "CoreMinimal.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FPGGameplayEffectContext : public FGameplayEffectContext
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "버프/디버프"))
    EPolarity Polarity = EPolarity::Buff;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "타깃 정책"))
    ESkillTargetPolicy TargetPolicy = ESkillTargetPolicy::Self;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "이펙트 태그"))
    FGameplayTagContainer EffectTags;

    virtual UScriptStruct* GetScriptStruct() const override;
    virtual FGameplayEffectContext* Duplicate() const override;
    virtual bool NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess) override;
};

template<>
struct TStructOpsTypeTraits<FPGGameplayEffectContext> : public TStructOpsTypeTraitsBase2<FPGGameplayEffectContext>
{
    enum
    {
        WithNetSerializer = true,
        WithCopy = true
    };
};
