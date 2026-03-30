// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Battle/UnitSlotWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "DataAssets/UI/UnitUIDataAsset.h"
#include "Character/Hero/HeroCharacter.h"
#include "Pawn/BaseStructure.h"
#include "Kismet/GameplayStatics.h"
#include "Mode/Save/PGGameInstance.h"
#include "UI/Battle/UnitSlotWidget.h"
#include "Character/Unit/SubSystem/UnitSpawnSubsystem.h"

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
    if (UnitData)
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
        UE_LOG(LogTemp, Warning, TEXT("유닛 데이터가 없음"));
        return;
    }

    AHeroCharacter* Hero = Cast<AHeroCharacter>(GetOwningPlayerPawn());
    if(Hero)
    {
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
            float RandomRange = FMath::RandRange(-250.0f, 250.0f);
            FVector FinalLocation = FVector(SpawnLocation.X + 200.0f, SpawnLocation.Y + RandomRange, 100.0f);
            FRotator SpawnRotation = FRotator::ZeroRotator;

            UUnitSpawnSubsystem* SpawnSystem = GetWorld()->GetSubsystem<UUnitSpawnSubsystem>();
            if (!SpawnSystem) return;

            AUnitCharacter* ReusedUnit = SpawnSystem->GetUnitInstance(UnitData->UnitClass);

            if (ReusedUnit)
            {
                ReusedUnit->SetActorLocationAndRotation(FinalLocation, SpawnRotation);

                int32 TargetLevel = 1;
                UPGGameInstance* GI = Cast<UPGGameInstance>(GetGameInstance());

                if (GI)
                {
                    if (FUnitSaveData* FoundData = GI->UnitLevelMap.Find(UnitData->UnitID))
                    {
                        if (FoundData->bIsUnlocked)
                        {
                            TargetLevel = FoundData->Level;
                        }
                    }
                }
                ReusedUnit->UnitLevel = TargetLevel;

                ReusedUnit->ActivateUnit();

                UE_LOG(LogTemp, Log, TEXT("Spawned(Reused) Unit: %d with Level: %d"), UnitData->UnitID, TargetLevel);
            }
        }
    }
}

