#pragma once

#include "IPropertyTypeCustomization.h"
#include "PropertyHandle.h"

/*
 * FAttributeModEntry 커스터마이저
 * Attribute <= ModifierOp <= ModifierValue 순으로 나오도록 하는 커스터마이저
 */

class FAttributeModEntryCustomization : public IPropertyTypeCustomization
{
public:
    static TSharedRef<IPropertyTypeCustomization> MakeInstance()
    {
        return MakeShareable(new FAttributeModEntryCustomization());
    }

    // IPropertyTypeCustomization 인터페이스 구현
    // Detail 패널의 해당 프로퍼티의 헤더를 커스터마이징하는 함수
    virtual void CustomizeHeader(
        TSharedRef<IPropertyHandle> PropertyHandle,
        FDetailWidgetRow& HeaderRow,
        IPropertyTypeCustomizationUtils& Utils) override;

    // IPropertyTypeCustomization 인터페이스 구현
    // 헤더 아래 펼쳐지는 자식 프로퍼티들을 커스터마이징하는 함수
    virtual void CustomizeChildren(
        TSharedRef<IPropertyHandle> PropertyHandle,
        IDetailChildrenBuilder& ChildBuilder,
        IPropertyTypeCustomizationUtils& Utils) override;
};
