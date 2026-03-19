// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Lobby/Stage/StageReadyWidget.h"
#include "Components/Button.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Mode/Save/PGGameInstance.h"
#include "DataAssets/UI/UnitUIDataAsset.h"
#include "DataAssets/UI/EnemyUIDataAsset.h"
#include "DataAssets/Spawner/DA_StageUnitListDataAsset.h"
#include "UI/Lobby/Stage/ReadyUnitWidget.h"
#include "UI/Lobby/Stage/ReadyEquipWidget.h"
#include "UI/Lobby/Stage/ReadyEnemyWidget.h"
#include "Kismet/GameplayStatics.h"

void UStageReadyWidget::NativeConstruct()
{
    Super::NativeConstruct();

    GI = Cast<UPGGameInstance>(GetGameInstance());

    if (ExitButton) ExitButton->OnClicked.AddDynamic(this, &UStageReadyWidget::OnExitButtonClicked);
    if (StartButton) StartButton->OnClicked.AddDynamic(this, &UStageReadyWidget::OnStartButtonClicked);
}

void UStageReadyWidget::OnExitButtonClicked()
{
    this->SetVisibility(ESlateVisibility::Hidden);
}

void UStageReadyWidget::OnStartButtonClicked()
{
    if (TargetLevel.IsNull()) return;

    // 레벨 에셋의 이름을 가져와서 해당 레벨을 열음
    FString LevelName = TargetLevel.GetAssetName();
    UGameplayStatics::OpenLevel(GetWorld(), FName(*LevelName));
}

void UStageReadyWidget::InitializeReadyUnit()
{
    TArray<TObjectPtr<UReadyUnitWidget>> UnitWidgets;
    UnitWidgets.Add(ReadyUnit1);
    UnitWidgets.Add(ReadyUnit2);
    UnitWidgets.Add(ReadyUnit3);
    UnitWidgets.Add(ReadyUnit4);
    UnitWidgets.Add(ReadyUnit5);

    for (int32 i = 0; i < GI->CurrentUnits.Num(); ++i)
    {
        if (!UnitWidgets.IsValidIndex(i) || !UnitWidgets[i]) break;

        UUnitUIDataAsset* UnitData = GI->CurrentUnits[i].LoadSynchronous();
        if (UnitData)
        {
            // UnitID를 통해 저장된 레벨 정보 가져오기
            int32 Level = 1;
            if (GI->UnitLevelMap.Contains(UnitData->UnitID))
            {
                Level = GI->UnitLevelMap[UnitData->UnitID].Level;
            }

            UnitWidgets[i]->UpdateUnitWidget(UnitData, Level);
        }
    }
}

void UStageReadyWidget::InitializeReadyEquip()
{
    ReadyWeapon->UpdateEquipWidget(GI->CurrentWeapon.LoadSynchronous());
    ReadyArmor->UpdateEquipWidget(GI->CurrentArmor.LoadSynchronous());
    ReadyAccessory->UpdateEquipWidget(GI->CurrentAccessory.LoadSynchronous());
}

void UStageReadyWidget::InitializeReadyEnemy(UDA_StageUnitListDataAsset* InEnemyListData)
{
    if (!EnemyBox || !EnemySlotClass || !InEnemyListData) return;

    // 기존 목록 초기화
    EnemyBox->ClearChildren();

    // 데이터 에셋의 UnitSpawnLis 순회
    for (const FUnitSpawnDataInfo& SpawnInfo : InEnemyListData->UnitSpawnList)
    {
        // 구조체 내부의 UnitData가 유효한지 확인
        if (SpawnInfo.UnitData)
        {
            UReadyEnemyWidget* NewEnemyWidget = CreateWidget<UReadyEnemyWidget>(this, EnemySlotClass);
            if (NewEnemyWidget)
            {
                // 적 위젯에 실제 UI 데이터 주입
                NewEnemyWidget->UpdateEnemyWidget(SpawnInfo.UnitData);
                UHorizontalBoxSlot* EnemySlot = EnemyBox->AddChildToHorizontalBox(NewEnemyWidget);
                if (EnemySlot)
                {
                    EnemySlot->SetPadding(FMargin(10.0f));
                }
            }
        }
    }
}

void UStageReadyWidget::InitializeReadyReward(const FStageDataTable& InStageData)
{
    // 2성, 3성 문구 생성
    FText Star2Text = GetRewardText(InStageData.RewardType, InStageData.Star2);
    FText Star3Text = GetRewardText(InStageData.RewardType, InStageData.Star3);

    if (Star2Description) Star2Description->SetText(Star2Text);
    if (Star3Description) Star3Description->SetText(Star3Text);
    if (RewardGem) RewardGem->SetText(FText::AsNumber(InStageData.RewardGem));
    if (RewardGold) RewardGold->SetText(FText::AsNumber(InStageData.RewardGold));
}

FText UStageReadyWidget::GetRewardText(ERewardCategory Category, float RewardValue)
{
    FString RewardString;

    switch (Category)
    {
    case ERewardCategory::Time:
        RewardString = FString::Printf(TEXT("%.0f초 이내로 클리어"), RewardValue);
        break;

    case ERewardCategory::Health:
        RewardString = FString::Printf(TEXT("아군 기지 체력 %.0f%% 이상"), RewardValue);
        break;

    case ERewardCategory::Cost:
        RewardString = FString::Printf(TEXT("소모 코스트 %.0f 이내로 클리어"), RewardValue);
        break;

    default:
        RewardString = TEXT("조건 정보 없음");
        break;
    }

    return FText::FromString(RewardString);
}

void UStageReadyWidget::InitializeReadyWidget(const FStageDataTable& InStageData)
{
    TargetLevel = InStageData.StageLevel;
    InitializeReadyUnit();
    InitializeReadyEquip();
    InitializeReadyEnemy(InStageData.EnemyList);
    InitializeReadyReward(InStageData);
}