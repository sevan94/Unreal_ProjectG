// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "GameplayTagContainer.h"
#include "GameplayEffectTypes.h"
#include "CoreMinimal.h"
#include "PGGameplayEffectContext.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FPGGameplayEffectContext : public FGameplayEffectContext
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "큐 스위처", Categories = "GameplayCue"))
    FGameplayTag CueVariantTag;

    // 이 컨텍스트의 실제 타입이 FPGGameplayEffectContext임을 엔진에 알려준다.
    virtual UScriptStruct* GetScriptStruct() const override;

    // 스펙 복사 / 재적용 때 컨텍스트를 깊은 복사합니다. (HitResult 같은 내부 데이터 보존)
    virtual FGameplayEffectContext* Duplicate() const override;

    // 네트워크로 보낼 때 커스텀 필드(EffectTag 등)를 직렬화/역직렬화합니다.
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
