// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameplayTagContainer.h"
#include "ControlPanelWidget.generated.h"

class UImage;
class UBarWidget;
class UBaseHpWidget;
class UActiveSkillWidget;
class AHeroCharacter;

/**
 * 
 */
UCLASS()
class UNREAL_PROJECTG_API UControlPanelWidget : public UUserWidget
{
	GENERATED_BODY()

public:
    // 외부(캐릭터)에서 조이스틱 값을 가져갈 함수
    FVector2D GetJoystickVector() const { return JoystickVector; }

    // 
    UFUNCTION()
    void UpdateHeroHP(float InValue);
    UFUNCTION()
    void UpdateMaxHeroHP(float InValue);
    UFUNCTION()
    void UpdateCost(float InValue);
    UFUNCTION()
    void UpdateMaxCost(float InValue);
    UFUNCTION()
    void UpdateBaseHP(FGameplayTag TeamTag, float InValue);
    UFUNCTION()
    void UpdateBaseMaxHP(FGameplayTag TeamTag, float InValue);

    // 스킬 테스트용 함수
    UFUNCTION(BlueprintCallable)
    void SetAbilitySpecHandle();

protected:
    // 블루프린트 이벤트를 C++에서 오버라이드
    virtual void NativeConstruct() override;
    // 마우스 버튼 클릭 시
    virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
    // 마우스 버튼 드래그 시
    virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
    // 마우스 버튼 클릭 해제 시
    virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;


protected:
    TObjectPtr<AHeroCharacter> HeroCharacter = nullptr;

    // 조이스틱 레버
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UImage> JoyStick;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UImage> JoyStickBackground;

    // 영웅 상태 바
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UBarWidget> HPBar;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UBarWidget> CostBar;

    // 기지 체력 바
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UBaseHpWidget> PlayerHP;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UBaseHpWidget> EnemyHP;

    // 액티브 스킬
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UActiveSkillWidget> WeaponSkill;
    
private:
    // 내부 변수
    bool bIsAreaPressed = false;
    float JoystickRange = 150.0f; // 블루프린트의 Clamp 값
    FVector2D JoystickVector = FVector2D::ZeroVector;

    UPROPERTY()
    TObjectPtr<AHeroCharacter> Hero = nullptr;
};
