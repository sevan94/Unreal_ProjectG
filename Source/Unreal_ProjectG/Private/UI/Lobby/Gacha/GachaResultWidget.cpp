// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Lobby/Gacha/GachaResultWidget.h"
#include "Components/WidgetSwitcher.h"
#include "Components/CanvasPanel.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "NiagaraSystemWidget.h"
#include "Animation/WidgetAnimation.h"

void UGachaResultWidget::InitUnitData(UUnitUIDataAsset* InData)
{
    if(InData) PickupUnit = InData;

    if (PickupUnit)
    {
        UnitImage->SetVisibility(ESlateVisibility::Visible);
        UnitImage->SetBrushFromTexture(PickupUnit->UnitImage);
    }
}

void UGachaResultWidget::InitEquipData(UEquipUIDataAsset* InData)
{
    bCanExit = false;
    if (InData) PickupEquip = InData;

    if (PickupEquip)
    {
        GachaReaultPanel->SetVisibility(ESlateVisibility::Visible);
        EquipGachaTarget->SetVisibility(ESlateVisibility::Visible);
        SkipButton->SetVisibility(ESlateVisibility::Hidden);
        EquipImage->SetBrushFromTexture(PickupEquip->EquipImage);
    }
}

void UGachaResultWidget::PlayGachaAnim()
{
    bCanExit = false; // 재생 시작 시 클릭 방지
    GachaReaultPanel->SetVisibility(ESlateVisibility::Visible);

    // 애니메이션 종료 델리게이트 바인딩
    FWidgetAnimationDynamicEvent EndEvent;
    EndEvent.BindDynamic(this, &UGachaResultWidget::OnGachaAnimFinished);
    BindToAnimationFinished(UnitGacha, EndEvent);

    PlayAnimation(UnitGacha);
    GachaEffect->ActivateSystem(false);
}

void UGachaResultWidget::ShowEquipResult()
{
    EquipImage->SetVisibility(ESlateVisibility::Visible);
    bCanExit = true;
}

void UGachaResultWidget::NativeConstruct()
{
    GachaReaultPanel->SetVisibility(ESlateVisibility::Hidden);
    UnitImage->SetVisibility(ESlateVisibility::Hidden);
    EquipImage->SetVisibility(ESlateVisibility::Hidden);
    EquipGachaTarget->SetVisibility(ESlateVisibility::Hidden);

    if (SkipButton) SkipButton->OnClicked.AddDynamic(this, &UGachaResultWidget::OnSkipButtonClicked);
}

FReply UGachaResultWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    if (bCanExit)
    {
        ResetGachaResult();
        return FReply::Handled();
    }
    return FReply::Unhandled();
}

void UGachaResultWidget::OnSkipButtonClicked()
{
    if (GachaReaultPanel && UnitGacha)
    {
        UnitImage->SetColorAndOpacity(FLinearColor::White);
        GachaReaultPanel->SetVisibility(ESlateVisibility::Visible);
        // 애니메이션을 마지막 프레임으로 이동
        float EndTime = UnitGacha->GetEndTime();

        // 애니메이션 재생 중일 수 있으므로 중지 후 시간 설정
        PauseAnimation(UnitGacha);
        SetAnimationCurrentTime(UnitGacha, EndTime);

        // 스킵 버튼 숨기기
        if (SkipButton)
        {
            SkipButton->SetVisibility(ESlateVisibility::Hidden);
        }

        // 플래그 허용
        bCanExit = true;
    }
}

void UGachaResultWidget::OnGachaAnimFinished()
{
    if (SkipButton)
    {
        SkipButton->SetVisibility(ESlateVisibility::Hidden);
    }
    bCanExit = true;
}

void UGachaResultWidget::ResetGachaResult()
{
    // 상태 초기화
    bCanExit = false;
    UnitImage->SetColorAndOpacity(FLinearColor::Black);
    UnitImage->SetVisibility(ESlateVisibility::Hidden);
    EquipImage->SetVisibility(ESlateVisibility::Hidden);
    EquipGachaTarget->SetVisibility(ESlateVisibility::Hidden);
    GachaReaultPanel->SetVisibility(ESlateVisibility::Hidden);
    GachaEffect->DeactivateSystem();
    SkipButton->SetVisibility(ESlateVisibility::Visible);

    // 애니메이션 정지 및 시간 초기화
    StopAnimation(UnitGacha);

    // 가챠 메인 화면으로 이동
    if (WidgetSwitcher) WidgetSwitcher->SetActiveWidgetIndex(4);
}
