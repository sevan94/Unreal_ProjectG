#pragma once

#include "IPropertyTypeCustomization.h"
#include "IPropertyUtilities.h"
#include "PropertyHandle.h"
#include "ClassViewerFilter.h"

//블루 프린트 전용 클래스 필터
class FBlueprintOnlyAbilityClassFilter : public IClassViewerFilter
{
public:
    //허용할 부모 클래스 목록
    TSet<const UClass*> AllowedChildrenOfClasses;

    // 클래스 뷰어가 Ui에서 목록을 표시하기 전에, 존재하는 모든 클래스를 이 함수에 전달하여, 필터링
    // 이 함수가 True면 UI에 표시, False면 UI에서 숨김
    virtual bool IsClassAllowed(
        const FClassViewerInitializationOptions& InInitOptions,     // 클래스 뷰어 초기화 옵션
        const UClass* InClass,                                      // 검사할 클래스
        TSharedRef<FClassViewerFilterFuncs> InFilterFuncs) override // 클래스가 허용되는지 여부를 반환하는 함수
    {
        // 블루프린트 컴파일 클래스만 허용
        if (!InClass->HasAnyClassFlags(CLASS_CompiledFromBlueprint))
        {
            return false;
        }

        // 지정된 부모의 자식 클래스인지 확인
        return InFilterFuncs->IfInChildOfClassesSet(
            AllowedChildrenOfClasses, InClass) != EFilterReturn::Failed;
    }
    virtual bool IsUnloadedClassAllowed(
        const FClassViewerInitializationOptions& InInitOptions,
        const TSharedRef< const IUnloadedBlueprintData > InUnloadedClassData,
        TSharedRef< FClassViewerFilterFuncs > InFilterFuncs) override
    {
        // 아직 로드되지 않은 블루프린트도 필터링
        return InFilterFuncs->IfInChildOfClassesSet(
            AllowedChildrenOfClasses, InUnloadedClassData) != EFilterReturn::Failed;
    }
};

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
    void UpdateConfigStructFilter();

    // 현재 선택된 어빌리티 클래스에 맞는 Config만 나오도록 하는 함수
    UScriptStruct* GetRequiredConfigStruct() const;

    // Config 가시성 반환
    EVisibility GetConfigVisibility() const;

    // 현재 선택된 UClass* 반환 (위젯 바인딩용)
    const UClass* GetCurrentAbilityClass() const;

    // 위젯에서 클래스 선택 시 핸들에 저장
    void OnAbilityClassSelected(const UClass* NewClass);

private:
    TSharedPtr<IPropertyHandle> AbilityClassHandle;
    TSharedPtr<IPropertyHandle> AbilityConfigHandle;

    // 패널 업데이트를 위해 Utils 저장
    TSharedPtr<IPropertyUtilities> PropertyUtilities;

    //bool bIsRefreshing = false;
};
