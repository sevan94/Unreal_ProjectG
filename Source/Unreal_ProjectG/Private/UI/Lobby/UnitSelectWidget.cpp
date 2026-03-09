// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Lobby/UnitSelectWidget.h"
#include "UI/UnitEntryObject.h"
#include "DataAssets/UI/UnitUIDataAsset.h"
#include "Components/Image.h"
#include "Components/Overlay.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Mode/Save/PGGameInstance.h"

void UUnitSelectWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
    UUnitEntryObject* EntryObject = Cast<UUnitEntryObject>(ListItemObject);

    if (EntryObject)
    {
        // 기존 바인딩이 있다면 제거 (재사용 시 중복 방지)
        EntryObject->OnUnitDataChanged.RemoveAll(this);

        // 데이터가 바뀌었을 때 UpdateWidget을 실행하도록 바인딩
        EntryObject->OnUnitDataChanged.AddDynamic(this, &UUnitSelectWidget::ListDataChanged);

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

void UUnitSelectWidget::ListDataChanged()
{
    UUnitEntryObject* EntryObject = Cast<UUnitEntryObject>(GetListItem());
    if (EntryObject)
    {
        UpdateWidget(EntryObject);
    }
}

void UUnitSelectWidget::UpdateWidget(UUnitEntryObject* InEntryObject)
{
    if (!InEntryObject) return;

    // EntryObject 내부의 데이터 에셋 가져오기
    UUnitUIDataAsset* UIData = InEntryObject->GetUnitUIData();
    UPGGameInstance* GI = Cast<UPGGameInstance>(GetGameInstance()); 
    FUnitSaveData SaveData = GI->GetUnitSaveData(UIData->UnitID);

    if (UIData && UnitImage)
    {
        // 이미지 위젯에 텍스처 설정
        UnitImage->SetBrushFromTexture(UIData->UnitImage);
        UnitLevel->SetText(FText::AsNumber(SaveData.Level));
    }

    // 소유 여부에 따른 잠금 표시 처리
    if (LockOverlay && SaveData.bIsUnlocked)
    {
        ESlateVisibility LockVisibility = InEntryObject->IsOwned() ? ESlateVisibility::Hidden : ESlateVisibility::HitTestInvisible;
        LockOverlay->SetVisibility(LockVisibility);
    }
}
