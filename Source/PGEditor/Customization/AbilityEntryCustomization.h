#pragma once

#include "IPropertyTypeCustomization.h"
#include "IPropertyUtilities.h"
#include "PropertyHandle.h"
/**
* FAbilityEntry 커스터마이저
* AbilityClass 선택 시 알맞은 어빌리티 Config만 나오도록 하는 커스터마이저
*/
class FAbilityEntryCustomization : public IPropertyTypeCustomization
{
public:
    static TSharedRef<IPropertyTypeCustomization> MakeInstance()
    {
        return MakeShareable(new FAbilityEntryCustomization());
    }

    // CustomizeHeader에서는 AbilityClass에 맞는 Config만 나오도록 하는 로직
    virtual void CustomizeHeader(
        TSharedRef<IPropertyHandle> PropertyHandle,
        FDetailWidgetRow& HeaderRow,
        IPropertyTypeCustomizationUtils& Utils) override;

    // CustomizeChildren 실행 타이밍은 
    virtual void CustomizeChildren(
        TSharedRef<IPropertyHandle> PropertyHandle,
        IDetailChildrenBuilder& ChildBuilder,
        IPropertyTypeCustomizationUtils& Utils) override;

private:
    // 어빌리티 클래스가 변경될 때마다 호출되는 함수
    void OnAbilityClassChanged();

    // 어빌리티 클래스에 맞는 Config만 나오도록 하는 필터 함수
    void UpdateConfigClassFilter();

    // 현재 선택된 어빌리티 클래스에 맞는 Config만 나오도록 하는 함수
    UClass* GetRequiredConfigClass() const;

    // Config 가시성 반환
    EVisibility GetConfigVisibility() const;

private:
    TSharedPtr<IPropertyHandle> AbilityClassHandle;
    TSharedPtr<IPropertyHandle> AbilityConfigHandle;

    // 패널 업데이트를 위해 Utils 저장
    TSharedPtr<IPropertyUtilities> PropertyUtilities;
};
