#include "AttributeModEntryCustomization.h"
#include "DetailWidgetRow.h"
#include "IDetailChildrenBuilder.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Input/STextComboBox.h"
#include "PropertyCustomizationHelpers.h"
#include "Engine/CurveTable.h"
#include "Types/PGGasTypes.h"

void FAttributeModEntryCustomization::CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& Utils)
{
    // 자식 프로퍼티 핸들 가져오기
    AttributeHandle = PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FPGAttributeModifierEntry, Attribute));
    ModifierOpHandle = PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FPGAttributeModifierEntry, ModifierOp));
    ModifierValueHandle = PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FPGAttributeModifierEntry, ModifierValue));


    //FScalableFloat 내부 핸들 가져오기
    ScalableValueHandle = ModifierValueHandle->GetChildHandle(TEXT("Value"));
    TSharedPtr<IPropertyHandle> CurveHandle = ModifierValueHandle->GetChildHandle(TEXT("Curve"));
    CurveTableHandle = CurveHandle->GetChildHandle(TEXT("CurveTable"));
    RowNameHandle = CurveHandle->GetChildHandle(TEXT("RowName"));

    // CurveTable 변경 감지 등록, Create SharedPointer로 바인딩
    CurveTableHandle->SetOnPropertyValueChanged(FSimpleDelegate::CreateSP(this, &FAttributeModEntryCustomization::OnCurveTableChanged));

    // 초기 RowName 옵션 설정
    RefreshRowNameOptions();

    // 헤더 한 줄에 자식 프로퍼티들을 순서대로 배치
    HeaderRow.NameContent()
        [
            PropertyHandle->CreatePropertyNameWidget()
        ]
        .ValueContent()
        .MinDesiredWidth(TOptional<float>(600.f))
        [
            SNew(SVerticalBox)

                // 1행 : Attribute <= Op <= Value
                + SVerticalBox::Slot()
                .AutoHeight()
                .Padding(0, 1)
                [
                    SNew(SHorizontalBox)

                        // Attribute 프로퍼티 위젯
                        + SHorizontalBox::Slot()
                        .FillWidth(1.0f)
                        .VAlign(VAlign_Center)
                        .Padding(0, 0, 2, 0)
                        [
                            SNew(SGameplayAttributeWidget)
                                .OnAttributeChanged(this, &FAttributeModEntryCustomization::OnAttributeChanged)
                                .DefaultProperty(GetCurrentAttribute().GetUProperty())
                        ]

                        // <= 화살표 텍스트
                        + SHorizontalBox::Slot()
                        .AutoWidth()
                        .VAlign(VAlign_Center)
                        .Padding(4, 0)
                        [
                            SNew(STextBlock)
                                .Text(FText::FromString(TEXT("\x2190")))
                                .Font(FCoreStyle::GetDefaultFontStyle("Bold", 12))
                                .ColorAndOpacity(FSlateColor(FLinearColor(0.7f, 0.7f, 0.7f)))
                        ]

                        // ModifierOp 프로퍼티 위젯
                        + SHorizontalBox::Slot()
                        .AutoWidth()
                        .VAlign(VAlign_Center)
                        .Padding(2, 0)
                        [
                            ModifierOpHandle->CreatePropertyValueWidget()
                        ]

                        // <= 화살표 텍스트
                        + SHorizontalBox::Slot()
                        .AutoWidth()
                        .VAlign(VAlign_Center)
                        .Padding(4, 0)
                        [
                            SNew(STextBlock)
                                .Text(FText::FromString(TEXT("\x2190")))
                                .Font(FCoreStyle::GetDefaultFontStyle("Bold", 12))
                                .ColorAndOpacity(FSlateColor(FLinearColor(0.7f, 0.7f, 0.7f)))
                        ]

                        // ScalableValue 프로퍼티 위젯 (ModifierValue 대신)
                        + SHorizontalBox::Slot()
                        .FillWidth(0.3f)
                        .VAlign(VAlign_Center)
                        .Padding(2, 0, 0, 0)
                        [
                            ScalableValueHandle->CreatePropertyValueWidget()
                        ]
                ]

            // 2행 : CurveTable RowName 드롭다운
            + SVerticalBox::Slot()
                .AutoHeight()
                .Padding(0, 1)
                [
                    SNew(SHorizontalBox)

                        // 1행의 Attribute와 ModOp 영역만큼 빈 공간 확보
                        + SHorizontalBox::Slot()
                        .FillWidth(1.0f)
                        [
                            SNew(STextBlock)
                                .Text(FText::FromString(TEXT("Curve")))
                                .Font(IPropertyTypeCustomizationUtils::GetRegularFont())
                                .ColorAndOpacity(FSlateColor(FLinearColor(0.5f, 0.5f, 0.5f)))
                        ]

                        // CurveTable 피커
                        + SHorizontalBox::Slot()
                        .VAlign(VAlign_Center)
                        .Padding(4, 0, 2, 0)
                        [
                            SNew(SObjectPropertyEntryBox)
                                .PropertyHandle(CurveTableHandle)           // CurveTable 핸들 연결
                                .AllowedClass(UCurveTable::StaticClass())   // UCurveTable만 선택 가능
                                .AllowClear(true)                           
                        ]

                        // RowName 드롭다운
                        + SHorizontalBox::Slot()
                        .FillWidth(0.4f)
                        .VAlign(VAlign_Center)
                        .Padding(2, 0, 0, 0)
                        [
                            SAssignNew(RowNameComboBox, STextComboBox)
                                .OptionsSource(&RowNameOptions)                       // RowName 옵션 연결
                                .InitiallySelectedItem(GetCurrentRowNameSelection())  // 현재 선택된 RowName 설정
                                .OnSelectionChanged(this, &FAttributeModEntryCustomization::OnRowNameSelected)   // 선택 변경 콜백
                        ]
                ]
        ];
}

void FAttributeModEntryCustomization::CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle, IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& Utils)
{
    // 자식 프로퍼티는 헤더에서 모두 커스터마이징했으므로 기본 구현으로 둠 (아무것도 추가하지 않음)
}

// ======================================================================================
// 어트리뷰트 변경 콜백
// =======================================================================================
void FAttributeModEntryCustomization::BuildAttributeOptions()
{

}

FGameplayAttribute FAttributeModEntryCustomization::GetCurrentAttribute() const
{
    if (!AttributeHandle.IsValid() || !AttributeHandle->IsValidHandle()) return FGameplayAttribute();

    void* DataPtr = nullptr;
    if(AttributeHandle->GetValueData(DataPtr) == FPropertyAccess::Success && DataPtr)
    {
        // GetValueData로 가져온 void*를 FGameplayAttribute로 캐스팅하여 반환
        return *static_cast<FGameplayAttribute*>(DataPtr);
    }
    return FGameplayAttribute();
}

void FAttributeModEntryCustomization::OnAttributeSelected(TSharedPtr<FString> NewSelection, ESelectInfo::Type SelectInfo)
{

}

TSharedPtr<FString> FAttributeModEntryCustomization::GetCurrentAttributeSelection() const
{
    return TSharedPtr<FString>();
}

// ======================================================================================
// CurveTable 드롭다운 수동 갱신 및 RowName 옵션 관리
// =======================================================================================
void FAttributeModEntryCustomization::OnCurveTableChanged()
{
    //커브 테이블이 바뀌면 RowName 옵션 갱신
    if (RowNameHandle.IsValid() || RowNameHandle->IsValidHandle())
    {
        RowNameHandle->SetValue(FName(NAME_None)); // RowName 초기화 (선택 해제)
    }

    RefreshRowNameOptions();

    // 콤보박스 리프레시
    if (RowNameComboBox.IsValid())
    {
        RowNameComboBox->RefreshOptions();
    }
}

void FAttributeModEntryCustomization::RefreshRowNameOptions()
{
    RowNameOptions.Empty();

    // "None" 옵션 추가
    RowNameOptions.Add(MakeShared<FString>(TEXT("None")));
    
    if (!CurveTableHandle.IsValid() || !CurveTableHandle->IsValidHandle()) return;

    // 현재 선택된 CurveTable 가져오기
    UObject* CurveTableObj = nullptr;
    CurveTableHandle->GetValue(CurveTableObj);
    UCurveTable* CurveTable = Cast<UCurveTable>(CurveTableObj);

    if (!CurveTable) return;

    // CurveTable의 RowName들을 옵션으로 추가
    const TMap<FName, FRealCurve*>& RowMap = CurveTable->GetRowMap();
    for (const auto& Pair : RowMap)
    {
        RowNameOptions.Add(MakeShared<FString>(Pair.Key.ToString()));
    }
}

void FAttributeModEntryCustomization::OnRowNameSelected(TSharedPtr<FString> NewSelection, ESelectInfo::Type SelectInfo)
{
    if (!RowNameHandle.IsValid() || !RowNameHandle->IsValidHandle()) return;

    if(*NewSelection == TEXT("None"))
    {
        RowNameHandle->SetValue(FName(NAME_None)); // "None" 선택 시 RowName 초기화
    }
    else
    {
        RowNameHandle->SetValue(FName(**NewSelection)); // 선택된 RowName 설정
    }
}

TSharedPtr<FString> FAttributeModEntryCustomization::GetCurrentRowNameSelection() const
{
    if(!RowNameHandle.IsValid() || !RowNameHandle->IsValidHandle()) return nullptr;

    FName CurrentRowName;
    RowNameHandle->GetValue(CurrentRowName);

    // 현재 RowName과 일치하는 옵션을 찾아 반환
    FString CurrentStr = CurrentRowName.IsNone() ? TEXT("None") : CurrentRowName.ToString();

    for (const TSharedPtr<FString>& Option : RowNameOptions)
    {
        if (*Option == CurrentStr)
        {
            return Option;
        }
    }

    // 못 찾으면 첫 번째 (None) 반환
    return RowNameOptions.Num() > 0 ? RowNameOptions[0] : nullptr;
}