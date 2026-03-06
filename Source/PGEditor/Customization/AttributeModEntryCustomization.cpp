#include "AttributeModEntryCustomization.h"
#include "DetailWidgetRow.h"
#include "IDetailChildrenBuilder.h"
#include "Types/PGGasTypes.h"

void FAttributeModEntryCustomization::CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& Utils)
{
    // 인라인 커스텀 제거 : 기본 이름/값 렌더링 사용
    HeaderRow.NameContent()
        [
            PropertyHandle->CreatePropertyNameWidget()
        ]
        .ValueContent()
        .MinDesiredWidth(300.f)
        [
            PropertyHandle->CreatePropertyValueWidget()
        ];
}

void FAttributeModEntryCustomization::CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle, IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& Utils)
{
    TSharedPtr<IPropertyHandle> AttributeHandle = PropertyHandle->GetChildHandle(
        GET_MEMBER_NAME_CHECKED(FPGAttributeModifierEntry, Attribute));
    TSharedPtr<IPropertyHandle> ModifierOpHandle = PropertyHandle->GetChildHandle(
        GET_MEMBER_NAME_CHECKED(FPGAttributeModifierEntry, ModifierOp));
    TSharedPtr<IPropertyHandle> ModifierValueHandle = PropertyHandle->GetChildHandle(
        GET_MEMBER_NAME_CHECKED(FPGAttributeModifierEntry, ModifierValue));

    // 원형 유지 : 전부 AddProperty로 엔진 기본 커스터마이저 사용
    if (AttributeHandle.IsValid())
    {
        ChildBuilder.AddProperty(AttributeHandle.ToSharedRef())
            .DisplayName(FText::FromString(TEXT("어트리뷰트")));
    }

    if (ModifierOpHandle.IsValid())
    {
        ChildBuilder.AddProperty(ModifierOpHandle.ToSharedRef())
            .DisplayName(FText::FromString(TEXT("모디파이어 작동")));
    }

    if (ModifierValueHandle.IsValid())
    {
        ChildBuilder.AddProperty(ModifierValueHandle.ToSharedRef())
            .DisplayName(FText::FromString(TEXT("모디파이어 규모")));
    }
}