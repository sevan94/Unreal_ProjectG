#pragma once

#include "IPropertyTypeCustomization.h"
#include "PropertyHandle.h"
#include "AttributeSet.h"
#include "SSearchableComboBox.h"

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

private:
    // ==========================================================================
    // Attribute 피커 드롭 다운 연동
    // ===========================================================================
    void BuildAttributeOptions();
    FGameplayAttribute GetCurrentAttribute() const;
    void OnAttributeSelected(TSharedPtr<FString> NewSelection, ESelectInfo::Type SelectInfo);
    TSharedPtr<FString> GetCurrentAttributeSelection() const;


    // ==========================================================================
    // CurveTable <=> RowName 연동
    // ===========================================================================
    
    // CurveTable 변경 시 RowName 드롭다운 갱신
    // Add Property가 아닌 SVerticalBox만 사용하기 때문에 수동 갱신
    void OnCurveTableChanged();
    void RefreshRowNameOptions();

    // RowName 콤보박스 콜백
    void OnRowNameSelected(TSharedPtr<FString> NewSelection, ESelectInfo::Type SelectInfo);
    TSharedPtr<FString> GetCurrentRowNameSelection() const;

private:
    TSharedPtr<IPropertyHandle> AttributeHandle;
    TSharedPtr<IPropertyHandle> ModifierOpHandle;
    TSharedPtr<IPropertyHandle> ModifierValueHandle;

    // FScalableFloat 내부 핸들
    TSharedPtr<IPropertyHandle> ScalableValueHandle;
    TSharedPtr<IPropertyHandle> CurveTableHandle;
    TSharedPtr<IPropertyHandle> RowNameHandle;

    // Attribute 드롭 다운 데이터
    TArray<FGameplayAttribute> AllAttributes;
    TArray<TSharedPtr<FString>> AttributeOptions;
    TSharedPtr<SSearchableComboBox> AttributeComboBox;

    // RowName 드롭 다운 옵션 (멤버로 유지하여 콤보박스 참조 유지)
    TArray<TSharedPtr<FString>> RowNameOptions;
    //콤보박스 위젯 참조 (옵션 갱신 시 리프레시용)
    TSharedPtr<STextComboBox> RowNameComboBox;
};
