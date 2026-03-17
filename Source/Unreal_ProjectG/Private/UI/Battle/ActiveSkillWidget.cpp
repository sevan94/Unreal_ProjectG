// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Battle/ActiveSkillWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Overlay.h"
#include "Components/Image.h"
#include "AbilitySystemComponent.h"
#include "Character/Hero/HeroCharacter.h"
#include "TimerManager.h"
#include "UI/DataTable/SkillUIDataTable.h"

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
            if (AbilityObject->GetCooldownTags()->IsValid())
            {
                CooldownTag = AbilityObject->GetCooldownTags()->GetByIndex(0);
            }
            UE_LOG(LogTemp, Log, TEXT("어빌리티 : %s, 쿨다운 태그 : %s"), *Spec->Ability->GetName(), *CooldownTag.ToString());

            if (AbilitySystemComponent && CooldownTag.IsValid())
            {
                // 태그 변경 이벤트 등록
                TagChangedDelegateHandle = AbilitySystemComponent->RegisterGameplayTagEvent(CooldownTag, EGameplayTagEventType::NewOrRemoved)
                    .AddUObject(this, &UActiveSkillWidget::OnCoolDownTagChanged);
            }
        }
    }
}

void UActiveSkillWidget::SetSkillIcon(UTexture2D* InIcon)
{
    SkillIcon = InIcon;
    
    // 초기 이미지 설정
    UpdateSlot(true);
}

void UActiveSkillWidget::OnCoolDownTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
    bool bIsTimerActive = GetWorld()->GetTimerManager().IsTimerActive(CoolTimeTimerHandle);

    // 쿨타임이 0보다 크고 타이머가 가동중이지 않을 때
    if (NewCount > 0 && !bIsTimerActive)
    {
        // 쿨타임 오버레이 표시 및 타이머 시작
        if (CoolTimeOverlay) CoolTimeOverlay->SetVisibility(ESlateVisibility::Visible);
        UpdateSlot(true);

        GetWorld()->GetTimerManager().SetTimer(CoolTimeTimerHandle, this, &UActiveSkillWidget::UpdateCoolTimeProgress, 0.1f, true);
    }
    else if(NewCount == 0)
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

    if (ActiveButton) ActiveButton->OnClicked.AddDynamic(this, &UActiveSkillWidget::OnActiveButtonClicked);
    if (SkillBackground) SkillBackground->SetBrushFromTexture(SkillBackgroundImage);
}

void UActiveSkillWidget::OnActiveButtonClicked()
{
    FGameplayAbilitySpec* Spec = AbilitySystemComponent->FindAbilitySpecFromHandle(AbilitySpec);
    if (!Spec) return;

    //AbilitySystemComponent->TryActivateAbility(AbilitySpec);
    //UE_LOG(LogTemp, Log, TEXT("어빌리티 발동 : %s"), *Spec->Ability->GetName());
    //UpdateSlot(true);

    if (Spec->IsActive())
    {
        // 이미 실행 중이라면 취소
        AbilitySystemComponent->CancelAbilityHandle(AbilitySpec);
        UpdateSlot(true);
        //UE_LOG(LogTemp, Log, TEXT("어빌리티 취소 : %s"), *Spec->Ability->GetName());
    }
    else
    {
        // 실행 중이 아니라면 활성화
        AbilitySystemComponent->TryActivateAbility(AbilitySpec);
        UpdateSlot(false);
        //UE_LOG(LogTemp, Log, TEXT("어빌리티 활성화 : %s"), *Spec->Ability->GetName());
    }
}

void UActiveSkillWidget::UpdateSlot(bool bIsActivate)
{
    FButtonStyle NewStyle = ActiveButton->GetStyle();
    if (bIsActivate)
    {
        NewStyle.Normal.SetResourceObject(SkillIcon);
        NewStyle.Hovered.SetResourceObject(SkillIcon);
        NewStyle.Pressed.SetResourceObject(SkillIcon);
    }
    else
    {
        NewStyle.Normal.SetResourceObject(CancelIcon);
        NewStyle.Hovered.SetResourceObject(CancelIcon);
        NewStyle.Pressed.SetResourceObject(CancelIcon);
    }

    ActiveButton->SetStyle(NewStyle);
}
