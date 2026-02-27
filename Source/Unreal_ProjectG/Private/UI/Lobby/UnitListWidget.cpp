// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Lobby/UnitListWidget.h"
#include "UI/DataTable/UnitUIDataTable.h"
#include "UI/UnitEntryObject.h"
#include "Components/TileView.h"

void UUnitListWidget::NativeConstruct()
{
    Super::NativeConstruct();

    InitializeUnitList();
}

void UUnitListWidget::InitializeUnitList()
{
    if (!UnitTileView) return;
    UnitTileView->ClearListItems();

    if (!UnitDataTable)
    {
        UE_LOG(LogTemp, Error, TEXT("UnitDataTable is NULL! Check your Blueprint reference."));
        return;
    }

    if (UnitDataTable && EntryObjectClass)
    {
        TArray<FName> RowNames = UnitDataTable->GetRowNames();
        if (RowNames.Num() == 0)
        {
            UE_LOG(LogTemp, Warning, TEXT("UnitDataTable is found, but RowNames are empty."));
            return;
        }

        for (const FName& RowName : RowNames)
        {
            // NewObject로 데이터 컨테이너 생성
            UUnitEntryObject* NewEntry = NewObject<UUnitEntryObject>(this, EntryObjectClass);

            if (NewEntry)
            {
                // 행 데이터를 찾아서 데이터 에셋 주입 (구조체 타입에 맞게 캐스팅 필요)
                FUnitUIDataTable* Row = UnitDataTable->FindRow<FUnitUIDataTable>(RowName, "Unit Data");
                NewEntry->SetUnitUIData(Row->UnitData);
                NewEntry->OnUnitRequestSelected.AddDynamic(this, &UUnitListWidget::OnUnitClicked);

                // Tile View에 데이터 추가
                UnitTileView->AddItem(NewEntry);
            }
        }
    }
}

void UUnitListWidget::OnUnitClicked(UUnitUIDataAsset* SelectedData)
{
    if (OnUnitSelected.IsBound())
    {
        OnUnitSelected.Broadcast(SelectedData);
    }
}
