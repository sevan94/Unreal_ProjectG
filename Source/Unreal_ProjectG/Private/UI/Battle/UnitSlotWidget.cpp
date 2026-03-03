// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Battle/UnitSlotWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "DataAssets/UI/UnitUIDataAsset.h"
#include "Character/Hero/HeroCharacter.h"

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
    UnitImage->SetBrushFromTexture(UnitData->UnitImage);
}

void UUnitSlotWidget::UpdateSlot(float InCost)
{
    UnitButton->SetIsEnabled(InCost >= UnitData->UnitCost);
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
        UE_LOG(LogTemp, Warning, TEXT("유닛 데이터가 없습니다."));
        return;
    }

    AHeroCharacter* Hero = Cast<AHeroCharacter>(GetOwningPlayerPawn());
    if (Hero->ConsumeCost(UnitData->UnitCost))
    {
        FVector SpawnLocation = FVector(0.0f, 0.0f, 100.0f);
        FRotator SpawnRotation = FRotator::ZeroRotator;
        FActorSpawnParameters SpawnParams;

        // 유닛 스폰
        GetWorld()->SpawnActor<AUnitCharacter>(UnitData->UnitClass, SpawnLocation, SpawnRotation, SpawnParams);

        UE_LOG(LogTemp, Log, TEXT("Spawned Unit: %s"), *UnitData->UnitClass->GetName());
    }
}

