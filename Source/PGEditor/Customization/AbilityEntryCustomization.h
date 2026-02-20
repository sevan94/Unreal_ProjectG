#pragma once

#include "IPropertyTypeCustomization.h"
#include "PropertyHandle.h"
/**
* FAbilityEntry 커스터마이저
* AbilityClass 선택 시 알맞은 어빌리티 Config만 나오도록 하는 커스터마이저
*/
//, public TSharedFromThis<FAbilityEntryCustomization>
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

    // CustomizeChildren에서는 AbilityClass에 맞는 Config만 나오도록 하는 로직
    virtual void CustomizeChildren(
        TSharedRef<IPropertyHandle> PropertyHandle,
        IDetailChildrenBuilder& ChildBuilder,
        IPropertyTypeCustomizationUtils& Utils) override;

private:
    // 어빌리티 클래스가 변경될 때마다 호출되는 함수
    void OnAbilityClassChanged();

    // 어빌리티 클래스에 맞는 Config만 나오도록 하는 필터 함수
    bool OnShouldFilterAsset(const FAssetData& AssetData) const;

    // 현재 선택된 어빌리티 클래스에 맞는 Config만 나오도록 하는 함수
    UClass* GetRequiredConfigClass() const;

    // Config 가시성 반환
    EVisibility GetConfigVisibility() const;

private:
    TSharedPtr<IPropertyHandle> AbilityClassHandle;
    TSharedPtr<IPropertyHandle> AbilityConfigHandle;
};
