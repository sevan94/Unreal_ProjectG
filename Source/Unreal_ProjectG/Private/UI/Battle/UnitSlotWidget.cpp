// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Battle/UnitSlotWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "DataAssets/UI/UnitUIDataAsset.h"
#include "Character/Hero/HeroCharacter.h"
#include "Pawn/BaseStructure.h"
#include "Kismet/GameplayStatics.h"

void UUnitSlotWidget::InitializeSlot(UUnitUIDataAsset* InDataAsset)
{
    if (!InDataAsset) return;

    UnitData = InDataAsset;

    // 텍스트 설정
    if (UnitCost && UnitData)
    {
        UnitCost->SetText(FText::AsNumber(UnitData->UnitCost));
    }

    // 버튼 이미지 설정
    if(UnitData)
    {
        UnitImage->SetBrushFromTexture(UnitData->UnitImage);
    }
}

void UUnitSlotWidget::UpdateSlot(float InCost)
{
    if(UnitData)
    {
        UnitButton->SetIsEnabled(InCost >= UnitData->UnitCost);
    }
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
        if (!SpawnBase)
        {
            TArray<AActor*> FoundBases;
            UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABaseStructure::StaticClass(), FoundBases);

            for (AActor* BaseActor : FoundBases)
            {
                SpawnBase = Cast<ABaseStructure>(BaseActor);
                if (SpawnBase->GetTeamTag().MatchesTag(FGameplayTag::RequestGameplayTag(FName("Unit.Side.Ally"))))
                {
                    SpawnLocation = SpawnBase->GetActorLocation();
                }
            }
        }
        float RandomRange = FMath::RandRange(-100.0f, 100.0f);
        FVector RandomLocation = FVector(SpawnLocation.X + 200.0f, SpawnLocation.Y + RandomRange, 100.0f);
        FRotator SpawnRotation = FRotator::ZeroRotator;
        FActorSpawnParameters SpawnParams;

        // 유닛 스폰
        GetWorld()->SpawnActor<AUnitCharacter>(UnitData->UnitClass, RandomLocation, SpawnRotation, SpawnParams);

        UE_LOG(LogTemp, Log, TEXT("Spawned Unit: %s"), *UnitData->UnitClass->GetName());
    }
}

