// Fill out your copyright notice in the Description page of Project Settings.


#include "PGFunctionLibrary.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/PGAbilitySystemComponent.h"
#include "Interfaces/PawnCombatInterface.h"
#include "Components/Combat/PawnCombatComponent.h"
#include "Character/PGCharacterBase.h"
#include "AbilitySystem/PGCharacterAttributeSet.h"
#include "PGGameplayTags.h"

UPGAbilitySystemComponent* UPGFunctionLibrary::NativeGetPGASCFromActor(AActor* InActor)
{
    check(InActor);

    return CastChecked<UPGAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InActor));
}

bool UPGFunctionLibrary::NativeDoesActorHaveTag(AActor* InActor, FGameplayTag TagToCheck)
{
    UPGAbilitySystemComponent* ASC = NativeGetPGASCFromActor(InActor);

    return ASC->HasMatchingGameplayTag(TagToCheck);
}

UPawnCombatComponent* UPGFunctionLibrary::NativeGetCombatComponentFromActor(AActor* InActor)
{
    check(InActor);

    if(IPawnCombatInterface* PawnCombatInterface = Cast<IPawnCombatInterface>(InActor))
    {
        return PawnCombatInterface->GetPawnCombatComponent();
    }

    if (UPawnCombatComponent* PawnCombatComp = InActor->FindComponentByClass<UPawnCombatComponent>())
    {
        return PawnCombatComp;
    }

    return nullptr;
}

UPawnCombatComponent* UPGFunctionLibrary::BP_GetCombatComponentFromActor(AActor* InActor, EPGValidType& OutValidType)
{
    UPawnCombatComponent* EquipComp = NativeGetCombatComponentFromActor(InActor);

    OutValidType = EquipComp ? EPGValidType::Valid : EPGValidType::InValid;
    return EquipComp;
}

bool UPGFunctionLibrary::ApplyGameplayEffectSpecHandleToTargetActor(AActor* InInstigator, AActor* InTargetActor, const FGameplayEffectSpecHandle& InSpecHandle)
{
    UPGAbilitySystemComponent* TargetASC = NativeGetPGASCFromActor(InTargetActor);
    UPGAbilitySystemComponent* SourceASC = NativeGetPGASCFromActor(InInstigator);

    FActiveGameplayEffectHandle ActivateGameplayEffectHandle = SourceASC->ApplyGameplayEffectSpecToTarget(*InSpecHandle.Data, TargetASC);

    return ActivateGameplayEffectHandle.WasSuccessfullyApplied();
}

bool UPGFunctionLibrary::IsTargetCharacterHostile(AActor* InInstigator, AActor* InTargetActor)
{
    // 두 태그가 다르면 적대 관계로 간주
    APGCharacterBase* InstigatorCharacter = Cast<APGCharacterBase>(InInstigator);
    APGCharacterBase* TargetCharacter = Cast<APGCharacterBase>(InTargetActor);

    if(!InstigatorCharacter || !TargetCharacter)
    {
        return false;
    }

    return InstigatorCharacter->GetTeamTag() != TargetCharacter->GetTeamTag();
}

const TMap<FGameplayAttribute, FGameplayTag>& UPGFunctionLibrary::GetAttributeTagMap()
{
    // 람다를 이용한 정적 초기화
    // Static이므로 호출 시에 전역에서 딱 한번만 초기화 되고, 이후에는 초기화된 맵을 반환
    static TMap<FGameplayAttribute, FGameplayTag> Map = []()
    {
        // 임시 맵 생성
        TMap<FGameplayAttribute, FGameplayTag> M;

        // Map.Add를 래핑하여 간결하게 사용 가능하도록 람다 함수를 정의
        // 이후 Add만으로 Map에 항목을 추가할 수 있음
        auto Add = [&M](const FGameplayAttribute& Attribute, const FGameplayTag& TagName)
        {
            M.Add(Attribute, TagName);
        };

        // ==========================================================================
        // 여기에 Attribute와 TagName을 추가
        // ==========================================================================
        Add(UPGCharacterAttributeSet::GetMaxHealthAttribute(), PGGameplayTags::Shared_SetByCaller_MaxHealthMultiplier);
        Add(UPGCharacterAttributeSet::GetAttackPowerAttribute(), PGGameplayTags::Shared_SetByCaller_AttackPowerMultiplier);
    
        return M;
    }();

    // 초기화된 맵 반환
    return Map;
}

FGameplayTag UPGFunctionLibrary::GetSetByCallerTagForAttribute(const FGameplayAttribute& Attribute)
{
    if (const FGameplayTag* FoundTag = GetAttributeTagMap().Find(Attribute))
    {
        return *FoundTag;
    }
    
    return FGameplayTag();
}
