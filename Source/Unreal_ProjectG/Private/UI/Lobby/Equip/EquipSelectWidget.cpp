// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Lobby/Equip/EquipSelectWidget.h"
#include "UI/EquipEntryObject.h"
#include "DataAssets/UI/EquipUIDataAsset.h"
#include "Components/Image.h"
#include "Components/Overlay.h"
#include "Components/Button.h"
#include "Mode/Save/PGGameInstance.h"

void UEquipSelectWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (EquipButton)
    {
        EquipButton->OnClicked.AddDynamic(this, &UEquipSelectWidget::OnEquipClicked);
    }
}

void UEquipSelectWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
    UEquipEntryObject* EntryObject = Cast<UEquipEntryObject>(ListItemObject);

    if (EntryObject)
    {
        UpdateWidget(EntryObject);
    }
}

void UEquipSelectWidget::OnEquipClicked()
{
    UEquipEntryObject* EntryObject = Cast<UEquipEntryObject>(GetListItem());
    if (EntryObject)
    {
        // 델리게이트 실행
        EntryObject->HandleClick();
    }
}

void UEquipSelectWidget::UpdateWidget(UEquipEntryObject* InEntryObject)
{
    if (!InEntryObject) return;

    // EntryObject 내부의 데이터 에셋 가져오기
    UEquipUIDataAsset* UIData = InEntryObject->GetEquipUIData();
    UPGGameInstance* GI = Cast<UPGGameInstance>(GetGameInstance());

    if (UIData && EquipImage)
    {
        // 이미지 위젯에 텍스처 설정
        EquipImage->SetBrushFromTexture(UIData->EquipImage);
    }

    // 소유 여부에 따른 잠금 표시 처리
    if (LockOverlay)
    {
        bool bIsOwned = GI->GetEquipOwned(UIData->EquipID);

        ESlateVisibility LockVisibility = bIsOwned ? ESlateVisibility::Hidden : ESlateVisibility::HitTestInvisible;

        LockOverlay->SetVisibility(LockVisibility);
    }
}
