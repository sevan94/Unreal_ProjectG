// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Lobby/EquipListWidget.h"
#include "Components/TileView.h"
#include "UI/EquipEntryObject.h"
#include "UI/DataTable/EquipUIDataTable.h"

void UEquipListWidget::UpdateList(EEquipCategory InCategory)
{
    if (!EquipTileView) return;

    // 기존 리스트 초기화
    EquipTileView->ClearListItems();

    // 해당 카테고리의 데이터 테이블 가져오기
    if (!EquipDataTables.Contains(InCategory)) return;
    UDataTable* TargetTable = EquipDataTables[InCategory];
    if (!TargetTable) return;

    // 데이터 테이블의 모든 행을 순회하며 EntryObject 생성
    static const FString ContextString(TEXT("EquipListContext"));
    TArray<FEquipUIDataTable*> AllRows;
    TargetTable->GetAllRows<FEquipUIDataTable>(ContextString, AllRows);

    for (FEquipUIDataTable* Row : AllRows)
    {
        if (Row && Row->EquipData)
        {
            // EntryObject 생성 (타일 뷰에 들어갈 데이터 컨테이너)
            UEquipEntryObject* NewEntry = NewObject<UEquipEntryObject>(this);
            NewEntry->SetEquipUIData(Row->EquipData);

            // 소유 여부 로직
            // NewEntry->SetIsOwned();

            // 클릭 이벤트 구독
            NewEntry->OnEquipRequestSelected.AddDynamic(this, &UEquipListWidget::OnItemClicked);

            // 타일 뷰에 아이템 추가
            EquipTileView->AddItem(NewEntry);
        }
    }
}

void UEquipListWidget::NativeConstruct()
{
    Super::NativeConstruct();
}

void UEquipListWidget::OnItemClicked(UEquipUIDataAsset* SelectedData)
{
    if (OnEquipSelected.IsBound())
    {
        // 리스트 내 아이템 클릭 시 브로드캐스트
        OnEquipSelected.Broadcast(SelectedData);
    }
}
