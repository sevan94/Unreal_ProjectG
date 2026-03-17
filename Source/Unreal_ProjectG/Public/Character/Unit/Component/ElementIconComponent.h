// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BillboardComponent.h"
#include "GameplayTagContainer.h"
#include "ElementIconComponent.generated.h"

USTRUCT(BlueprintType)
struct FUnitIconData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FGameplayTag BranchTag;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FGameplayTag ElementTag;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UTexture2D* IconTexture;
};
/**
 * 
 */
UCLASS(Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UNREAL_PROJECTG_API UElementIconComponent : public UBillboardComponent
{
	GENERATED_BODY()
public:
    virtual void BeginPlay() override;

    UElementIconComponent();

    UFUNCTION()
    void HandleUnitDataLoaded();

    // 12가지 조합을 입력할 배열
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit Icon")
    TArray<FUnitIconData> IconMappings;

    // 태그를 확인하고 이미지를 변경하는 함수
    UFUNCTION(BlueprintCallable, Category = "Unit Icon")
    void UpdateOverheadIcon(FGameplayTag CurrentBranch, FGameplayTag CurrentElement);
};
