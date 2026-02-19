// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Battle/ActiveSkillWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Overlay.h"
#include "AbilitySystemComponent.h"
#include "Character/Hero/HeroCharacter.h"
#include "Components/Combat/PawnCombatComponent.h"
#include "TimerManager.h"

void UActiveSkillWidget::SetAbilitySpecHandle(FGameplayAbilitySpecHandle InHandle)
{
    AbilitySpec = InHandle;

    if (AbilitySystemComponent && AbilitySpec.IsValid())
    {
        FGameplayAbilitySpec* Spec = AbilitySystemComponent->FindAbilitySpecFromHandle(AbilitySpec);

        if (Spec)
        {
            AbilityObject = Spec->GetPrimaryInstance();
            if (!AbilityObject)
            {
                AbilityObject = Spec->Ability;
            }
            FString AbilityName = Spec->Ability->GetName();
            CooldownTag = AbilityObject->GetCooldownTags()->GetByIndex(0);
            UE_LOG(LogTemp, Log, TEXT("어빌리티(%s)로부터 쿨다운 태그(%s)를 자동으로 가져왔습니다."),
                *Spec->Ability->GetName(), *CooldownTag.ToString());

            if (AbilitySystemComponent && CooldownTag.IsValid())
            {
                // 태그 변경 이벤트 등록
                TagChangedDelegateHandle = AbilitySystemComponent->RegisterGameplayTagEvent(CooldownTag, EGameplayTagEventType::NewOrRemoved)
                    .AddUObject(this, &UActiveSkillWidget::OnCoolDownTagChanged);
            }
        }
    }
}

void UActiveSkillWidget::OnCoolDownTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
    float CoolTimeRemaining = AbilityObject->GetCooldownTimeRemaining();
    bool bIsTimerActive = GetWorld()->GetTimerManager().IsTimerActive(CoolTimeTimerHandle);

    // 쿨타임이 0보다 크고 타이머가 가동중이지 않을 때
    if (CoolTimeRemaining > 0 && !bIsTimerActive)
    {
        // 쿨타임 오버레이 표시 및 타이머 시작
        if (CoolTimeOverlay) CoolTimeOverlay->SetVisibility(ESlateVisibility::Visible);

        GetWorld()->GetTimerManager().SetTimer(CoolTimeTimerHandle, this, &UActiveSkillWidget::UpdateCoolTimeProgress, 0.1f, true);
    }
    else
    {
        // 쿨타임 종료 시 오버레이 숨김 및 타이머 정지
        if (CoolTimeOverlay) CoolTimeOverlay->SetVisibility(ESlateVisibility::Collapsed);
        GetWorld()->GetTimerManager().ClearTimer(CoolTimeTimerHandle);
    }
}

void UActiveSkillWidget::UpdateCoolTimeProgress()
{
    float CoolTimeRemaining = AbilityObject->GetCooldownTimeRemaining();
    if (CoolTimeRemaining > 0.0f)
    {
        // FMath::CeilToInt -> 정수로 표기
        CoolTimeText->SetText(FText::AsNumber(FMath::CeilToInt(CoolTimeRemaining)));
    }
    else
    {
        // 텍스트 비우기
        if (CoolTimeText)
            CoolTimeText->SetText(FText::GetEmpty());

        // 오버레이 숨기기
        if (CoolTimeOverlay)
            CoolTimeOverlay->SetVisibility(ESlateVisibility::Collapsed);

        // 타이머 중지 및 핸들 초기화
        GetWorld()->GetTimerManager().ClearTimer(CoolTimeTimerHandle);
    }
}

void UActiveSkillWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // 쿨타임 오버레이 초기화
    CoolTimeOverlay->SetVisibility(ESlateVisibility::Collapsed);

    if (APawn* OwningPawn = GetOwningPlayerPawn())
    {
        AbilitySystemComponent = OwningPawn->FindComponentByClass<UAbilitySystemComponent>();
    }

    if (ActiveButton)
    {
        ActiveButton->OnClicked.AddDynamic(this, &UActiveSkillWidget::OnActiveButtonClicked);
    }
}

void UActiveSkillWidget::OnActiveButtonClicked()
{
    if (AbilitySystemComponent && AbilitySpec.IsValid())
    {
        AbilitySystemComponent->TryActivateAbility(AbilitySpec);
    }
}
