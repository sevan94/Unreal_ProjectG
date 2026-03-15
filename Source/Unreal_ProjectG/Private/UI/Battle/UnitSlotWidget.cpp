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

        // 유닛 인스턴스 생성
        AUnitCharacter* NewUnit = GetWorld()->SpawnActorDeferred<AUnitCharacter>(
            UnitData->UnitClass,
            FTransform(SpawnRotation, RandomLocation),
            nullptr, nullptr,
            ESpawnActorCollisionHandlingMethod::AlwaysSpawn
        );

        if (NewUnit)
        {
            int32 TargetID = UnitData->UnitID;
            int32 TargetLevel = 1;
            UPGGameInstance* GI = Cast<UPGGameInstance>(GetGameInstance());
          
            if (FUnitSaveData* FoundData = GI->UnitLevelMap.Find(TargetID))
            {
                // 유닛이 해금된 상태인지 확인 후 레벨 적용
                if (FoundData->bIsUnlocked)
                {
                    TargetLevel = FoundData->Level;
                }
            }
            NewUnit->UnitLevel = TargetLevel;

            // 인스턴스를 바탕으로 유닛 스폰
            NewUnit->FinishSpawning(FTransform(SpawnRotation, RandomLocation));

            UE_LOG(LogTemp, Log, TEXT("Spawned Unit: %d with Level: %d"), UnitData->UnitID, TargetLevel);
        }
    }
}

