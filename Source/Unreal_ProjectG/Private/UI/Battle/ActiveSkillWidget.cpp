// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Battle/ActiveSkillWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Overlay.h"
#include "Components/Image.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/Abilities/PGGameplayAbility.h"
#include "Character/Hero/HeroCharacter.h"
#include "TimerManager.h"
#include "UI/DataTable/SkillUIDataTable.h"

void UActiveSkillWidget::SetAbilitySpec(FGameplayAbilitySpec InSpec)
{
    if (!InSpec.Ability || !AbilitySystemComponent) return;

    // 핸들 저장
    AbilitySpecHandle = InSpec.Handle;

    // 스킬 데이터 에셋 저장
    SkillData = Cast<UDataAsset_HeroSkillData>(InSpec.SourceObject);

    // 어빌리티의 동적 태그를 확인하여 감시할 CooldownTag 결정
    const FGameplayTagContainer& DynamicTags = InSpec.GetDynamicSpecSourceTags();
    if (DynamicTags.HasTag(PGGameplayTags::Input_ActiveSkill_MainSkill))
    {
        CooldownTag = PGGameplayTags::Ability_Cooldown_MainSkill;
    }
    else if (DynamicTags.HasTag(PGGameplayTags::Input_ActiveSkill_SubSkill))
    {
        CooldownTag = PGGameplayTags::Ability_Cooldown_SubSkill;
    }

    // 쿨다운 태그 이벤트 바인딩
    if (CooldownTag.IsValid())
    {
        AbilitySystemComponent->RegisterGameplayTagEvent(CooldownTag, EGameplayTagEventType::NewOrRemoved).RemoveAll(this);
        AbilitySystemComponent->RegisterGameplayTagEvent(CooldownTag, EGameplayTagEventType::NewOrRemoved)
            .AddUObject(this, &UActiveSkillWidget::OnCoolDownTagChanged);
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
    if (NewCount > 0)
    {
        // 쿨타임 시작
        if (CoolTimeOverlay) CoolTimeOverlay->SetVisibility(ESlateVisibility::Visible);
        UpdateSlot(true);

        if (!GetWorld()->GetTimerManager().IsTimerActive(CoolTimeTimerHandle))
        {
            GetWorld()->GetTimerManager().SetTimer(CoolTimeTimerHandle, this, &UActiveSkillWidget::UpdateCoolTimeProgress, 0.1f, true);
        }
    }
    else
    {
        // 쿨타임 종료
        if (CoolTimeOverlay) CoolTimeOverlay->SetVisibility(ESlateVisibility::Collapsed);
        if (CoolTimeText) CoolTimeText->SetText(FText::GetEmpty());
        GetWorld()->GetTimerManager().ClearTimer(CoolTimeTimerHandle);
    }
}

void UActiveSkillWidget::UpdateCoolTimeProgress()
{
    if (!AbilitySystemComponent || !CooldownTag.IsValid()) return;

    //현재 태그에 대한 쿨타임 정보 가져오기
    FGameplayEffectQuery const Query = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(FGameplayTagContainer(CooldownTag));
    TArray<float> Times = AbilitySystemComponent->GetActiveEffectsTimeRemaining(Query);

    if (Times.Num() > 0)
    {
        // 가장 많이 남은 시간을 표시
        float MaxRemaining = 0.f;
        for (float Time : Times) { MaxRemaining = FMath::Max(MaxRemaining, Time); }
        if (MaxRemaining > 0.f)
        {
            if (CoolTimeText)
                CoolTimeText->SetText(FText::AsNumber(FMath::CeilToInt(MaxRemaining)));
        }
        else
        {
            // 시간이 다 됐으면 타이머 정리
            OnCoolDownTagChanged(CooldownTag, 0);
        }
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
    if (!AbilitySystemComponent || !AbilitySpecHandle.IsValid()) return;

    FGameplayAbilitySpec* Spec = AbilitySystemComponent->FindAbilitySpecFromHandle(AbilitySpecHandle);
    if (!Spec) return;

    bool bIsToggle = SkillData->bIsToggleSkill;

    // 이미 사용중인 상태라면
    if (Spec->IsActive())
    {
        if (bIsToggle)
        {
            // 토글형 스킬인 경우 취소 로직
            AbilitySystemComponent->CancelAbilityHandle(AbilitySpecHandle);
            UpdateSlot(true);
        }
    }
    else
    {
        // 사용 중이 아니라면 스킬 실행
        if (AbilitySystemComponent->TryActivateAbility(AbilitySpecHandle))
        {
            if (bIsToggle)
            {
                // 토글형 스킬인 경우 취소 위젯으로 전환
                UpdateSlot(false);
            }
            else
            {
                // 일반 스킬일 경우 이미지 변화 X
            }
        }
    }
}

void UActiveSkillWidget::UpdateSlot(bool bIsActivate)
{
    if (!ActiveButton) return;

    FButtonStyle NewStyle = ActiveButton->GetStyle();
    UTexture2D* TargetIcon = bIsActivate ? SkillIcon : CancelIcon;

    if (TargetIcon)
    {
        NewStyle.Normal.SetResourceObject(TargetIcon);
        NewStyle.Hovered.SetResourceObject(TargetIcon);
        NewStyle.Pressed.SetResourceObject(TargetIcon);
        ActiveButton->SetStyle(NewStyle);
    }
}
