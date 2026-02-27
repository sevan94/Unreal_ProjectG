// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Lobby/UnitSelectWidget.h"
#include "UI/UnitEntryObject.h"
#include "DataAssets/UI/UnitUIDataAsset.h"
#include "Components/Image.h"
#include "Components/Overlay.h"
#include "Components/Button.h"

void UUnitSelectWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
    UUnitEntryObject* EntryObject = Cast<UUnitEntryObject>(ListItemObject);

    if (EntryObject)
    {
        UpdateWidget(EntryObject);
        UnitButton->OnClicked.AddDynamic(this, &UUnitSelectWidget::OnUnitClicked);
    }
}

void UUnitSelectWidget::OnUnitClicked()
{
    UUnitEntryObject* EntryObject = Cast<UUnitEntryObject>(GetListItem());
    if (EntryObject)
    {
        // 델리게이트 실행
        EntryObject->HandleClick();
    }
}

void UUnitSelectWidget::UpdateWidget(UUnitEntryObject* InEntryObject)
{
    if (!InEntryObject) return;

    // EntryObject 내부의 데이터 에셋 가져오기
    UUnitUIDataAsset* UIData = InEntryObject->GetUnitUIData();

    if (UIData && UnitImage)
    {
        // 이미지 위젯에 텍스처 설정
        UnitImage->SetBrushFromTexture(UIData->UnitImage);
    }

    // 소유 여부에 따른 잠금 표시 처리
    if (LockOverlay)
    {
        ESlateVisibility LockVisibility = InEntryObject->IsOwned() ? ESlateVisibility::Hidden : ESlateVisibility::HitTestInvisible;
        LockOverlay->SetVisibility(LockVisibility);
    }
}
