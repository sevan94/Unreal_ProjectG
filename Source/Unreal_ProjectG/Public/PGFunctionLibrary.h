// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GameplayTagContainer.h"
#include "Types/PGEnumTypes.h"
#include "PGFunctionLibrary.generated.h"

class UPGAbilitySystemComponent;
class UPawnCombatComponent;
struct FGameplayEffectSpecHandle;
class APGCharacterBase;
struct FGameplayAttribute;
/**
 * 
 */
UCLASS()
class UNREAL_PROJECTG_API UPGFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
    static UPGAbilitySystemComponent* NativeGetPGASCFromActor(AActor* InActor);

    // 타겟 액터가 특정 태그를 가지고 있는지 확인하는 네이티브 함수
    static bool NativeDoesActorHaveTag(AActor* InActor, FGameplayTag TagToCheck);

    //UFUNCTION(BlueprintCallable, Category = "ProjectG|FunctionLibrary")
    //static void BP_DoesActorHaveTag(AActor* InActor, FGameplayTag TagToCheck, )

    static UPawnCombatComponent* NativeGetCombatComponentFromActor(AActor* InActor);

    UFUNCTION(BlueprintCallable, Category = "ProjectG|FunctionLibrary", meta = (DisplayName = "Get CombatComponent From Actor", ExpandEnumAsExecs = "OutValidType"))
    static UPawnCombatComponent* BP_GetCombatComponentFromActor(AActor* InActor, EPGValidType& OutValidType);

    // 게임플레이 이펙트 스펙 핸들을 타겟 액터에 적용하는 함수
    UFUNCTION(BlueprintCallable, Category = "ProjectG|FunctionLibrary")
    static bool ApplyGameplayEffectSpecHandleToTargetActor(AActor* InInstigator, AActor* InTargetActor, const FGameplayEffectSpecHandle& InSpecHandle);

    // 타겟 액터가 적대적인지 확인하는 함수
    UFUNCTION(BlueprintCallable, Category = "ProjectG|FunctionLibrary")
    static bool IsTargetCharacterHostile(AActor* InInstigator, AActor* InTargetActor);

    // Map<Attribute, FGameplayTag>로 등록된 전체 매핑 반환 (Effect 초기화용)
    static const TMap<FGameplayAttribute, FGameplayTag>& GetAttributeTagMap();

    // Attribute와 매핑된 SetByCaller 태그를 반환하는 함수
    static FGameplayTag GetSetByCallerTagForAttribute(const FGameplayAttribute& Attribute);
};
