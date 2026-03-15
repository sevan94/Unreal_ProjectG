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
        UE_LOG(LogTemp, Error, TEXT("유닛 데이터 테이블을 찾을 수 없습니다."));
        return;
    }

    if (UnitDataTable && EntryObjectClass)
    {
        TArray<FName> RowNames = UnitDataTable->GetRowNames();
        if (RowNames.Num() == 0)
        {
            UE_LOG(LogTemp, Warning, TEXT("유닛 데이터 테이블에 데이터가 없습니다."));
            return;
        }

        for (const FName& RowName : RowNames)
        {
            // EntryObject 생성 (타일 뷰에 들어갈 데이터 컨테이너)
            UUnitEntryObject* NewEntry = NewObject<UUnitEntryObject>(this, EntryObjectClass);

            if (NewEntry)
            {
                // 행 데이터를 찾아서 데이터 에셋 주입
                FUnitUIDataTable* Row = UnitDataTable->FindRow<FUnitUIDataTable>(RowName, "Unit Data");
                NewEntry->SetUnitUIData(Row->UnitData);

                // 클릭 이벤트 구독
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
        // 리스트 내 유닛 클릭 시 브로드 캐스트
        OnUnitSelected.Broadcast(SelectedData);
    }
}
