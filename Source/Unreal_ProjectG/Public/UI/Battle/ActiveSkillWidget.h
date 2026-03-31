// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameplayAbilities/public/Abilities/GameplayAbility.h"
#include "GamePlayAbilitySpecHandle.h"
#include "DataAssets/Ability/DataAsset_HeroSkillData.h"
#include "PGGameplayTags.h"
#include "ActiveSkillWidget.generated.h"

class UButton;
class UImage;
class UOverlay;
class UTextBlock;

/**
 * 
 */
UCLASS()
class UNREAL_PROJECTG_API UActiveSkillWidget : public UUserWidget
{
	GENERATED_BODY()

public:
    // 어빌리티 스펙 핸들을 이용하여 어빌리티 오브젝트 레퍼런스 할당
    UFUNCTION(BlueprintCallable)
    void SetAbilitySpec(FGameplayAbilitySpec InSpec);

    // 스킬 이미지 설정
    void SetSkillIcon(UTexture2D* InIcon);

    // 쿨타임 태그가 변경될 때 호출될 함수
    virtual void OnCoolDownTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

    // 쿨타임 UI 업데이트용
    void UpdateCoolTimeProgress();

protected:
    virtual void NativeConstruct() override;
    UFUNCTION()
    void OnActiveButtonClicked();

    void UpdateSlot(bool bIsActivate);

protected:
    // UI 바인딩
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> ActiveButton;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UOverlay> CoolTimeOverlay;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> CoolTimeText;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UImage> SkillBackground;

    UPROPERTY()
    TObjectPtr<UTexture2D> SkillIcon;
    UPROPERTY(EditAnywhere, Category = "Data")
    TObjectPtr<UTexture2D> SkillBackgroundImage;

    UPROPERTY(EditAnywhere, Category = "Data")
    TObjectPtr<UTexture2D> CancelIcon;

    FGameplayTag CooldownTag;

private:
    // 영웅 어빌리티 시스템
    TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

    // 어빌리티 스펙
    FGameplayAbilitySpecHandle AbilitySpecHandle;

    // 어빌리티 스킬 데이터
    TObjectPtr<UDataAsset_HeroSkillData> SkillData;

    // 스킬 쿨타임 타이머 핸들
    FTimerHandle CoolTimeTimerHandle;

    // 태그 변경 이벤트 바인딩 해제를 위한 핸들
    FDelegateHandle TagChangedDelegateHandle;
};
