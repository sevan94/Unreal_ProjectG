// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Battle/UnitSlotWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "DataAssets/UI/UnitUIDataAsset.h"

void UUnitSlotWidget::InitializeSlot(UUnitUIDataAsset* InDataAsset)
{
    if (!InDataAsset) return;

    UnitData = InDataAsset;

    // 텍스트 설정
    if (UnitCost)
    {
        UnitCost->SetText(FText::AsNumber(UnitData->UnitCost));
    }

    // 버튼 이미지 설정
    FButtonStyle NewStyle = UnitButton->GetStyle();

    if (UnitData->UnitButtonNormalImage)
    {
        NewStyle.Normal.SetResourceObject(UnitData->UnitButtonNormalImage);
        NewStyle.Hovered.SetResourceObject(UnitData->UnitButtonNormalImage);
    }

    if (UnitData->UnitButtonPressedImage)
        NewStyle.Pressed.SetResourceObject(UnitData->UnitButtonPressedImage);

    if (UnitData->UnitButtonUnableImage)
        NewStyle.Disabled.SetResourceObject(UnitData->UnitButtonUnableImage);

    UnitButton->SetStyle(NewStyle);
}

void UUnitSlotWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (UnitButton)
    {
        UnitButton->OnClicked.AddDynamic(this, &UUnitSlotWidget::OnUnitButtonClicked);
    }
}

void UUnitSlotWidget::OnUnitButtonClicked()
{
    if (!UnitData || !UnitData->UnitClass)
    {
        UE_LOG(LogTemp, Warning, TEXT("UnitData or UnitClass is null!"));
        return;
    }

    FVector SpawnLocation = FVector(0.0f, 0.0f, 100.0f);
    FRotator SpawnRotation = FRotator::ZeroRotator;
    FActorSpawnParameters SpawnParams;

    // 유닛 스폰
    GetWorld()->SpawnActor<AUnitCharacter>(UnitData->UnitClass, SpawnLocation, SpawnRotation, SpawnParams);

    UE_LOG(LogTemp, Log, TEXT("Spawned Unit: %s"), *UnitData->UnitClass->GetName());
}

