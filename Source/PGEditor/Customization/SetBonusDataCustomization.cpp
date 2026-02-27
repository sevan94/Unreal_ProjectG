// Fill out your copyright notice in the Description page of Project Settings.


#include "SetBonusDataCustomization.h"
#include "DetailLayoutBuilder.h"
#include "DetailCategoryBuilder.h"
#include "DetailWidgetRow.h"
#include "PropertyCustomizationHelpers.h"
#include "ClassViewerModule.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Images/SImage.h"

#include "PGEditor/Customization/AbilityEntryCustomization.h"
#include "DataAssets/Items/DataAsset_SetBonusData.h"
#include "AbilitySystem/Abilities/PGGameplayAbility.h"

void FSetBonusDataCustomization::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
    // 디테일 빌더의 유틸리티를 가져와서 디테일 패널과의 통신에 사용할 수 있도록 저장
    PropertyUtilities = DetailBuilder.GetPropertyUtilities();

    // 기본 프로퍼티 핸들 가져오기
    SetTagHandle = DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(UDataAsset_SetBonusData, SetTag));
    BaseAbilityHandle = DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(UDataAsset_SetBonusData, BaseAbility));
    UpgradedAbilityHandle = DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(UDataAsset_SetBonusData, UpgradedAbility));

    // 기본 프로퍼티 숨기기 (커스텀 UI로 대체)
    DetailBuilder.HideProperty(BaseAbilityHandle);
    DetailBuilder.HideProperty(UpgradedAbilityHandle);

    // 블루프린트 전용 어빌리티 클래스 필터 생성
    TSharedPtr<FBlueprintOnlyAbilityClassFilter> ClassFilter = MakeShared<FBlueprintOnlyAbilityClassFilter>();
    ClassFilter->AllowedChildrenOfClasses.Add(UPGGameplayAbility::StaticClass()); // 클래스의 자식 클래스만 허용하도록 설정

    TArray<TSharedRef<IClassViewerFilter>> ClassFilters; // IClassViewerFilter : 클래스를 필터링하는 인터페이스
    ClassFilters.Add(ClassFilter.ToSharedRef());

    // 어빌리티 업그레이드 카테고리 생성
    IDetailCategoryBuilder& UpgradeCategory = DetailBuilder.EditCategory(
        TEXT("어빌리티 업그레이드"),
        FText::GetEmpty(),
        ECategoryPriority::Important); // 카테고리 우선순위 설정

    // 디테일 창 UI를 커스텀
    // 화살표 형태의 커스텀 행 추가
    UpgradeCategory.AddCustomRow(FText::FromString(TEXT("어빌리티 업그레이드")))
    .WholeRowContent() // 커스텀 행이 전체 행을 차지하도록 설정
    [
        // 디테일 설명 위젯 행
        SNew(SVerticalBox) // 수직 박스 생성
            // 라벨 행
            + SVerticalBox::Slot()
            .AutoHeight()
            .Padding(0, 4, 0, 2)
            [
                SNew(SHorizontalBox) // 수평 박스 생성
                    + SHorizontalBox::Slot()
                    .FillWidth(1.0f)
                    .Padding(0, 0, 8, 0)
                    [
                        SNew(STextBlock)
                            .Text(FText::FromString(TEXT("업그레이드 전 어빌리티")))
                            .Font(IDetailLayoutBuilder::GetDetailFontBold())
                            .ColorAndOpacity(FLinearColor(0.8f, 0.4f, 0.4f)) // 붉은색
                    ]
    
                    + SHorizontalBox::Slot()
                    .AutoWidth()
                    .VAlign(VAlign_Center)
                    .Padding(4, 0, 4, 0)
                    [
                        SNew(SBox)
                            .WidthOverride(40.f)
                            [
                                SNew(STextBlock)
                                    .Text(FText::FromString(TEXT("")))
                                    .Justification(ETextJustify::Center)
                            ]
                    ]
    
                + SHorizontalBox::Slot()
                    .FillWidth(1.0f)
                    .Padding(8, 0, 0, 0) // 왼쪽으로 8의 패딩
                    [
                        SNew(STextBlock)
                            .Text(FText::FromString(TEXT("업그레이드 후 어빌리티")))
                            .Font(IDetailLayoutBuilder::GetDetailFontBold())
                            .ColorAndOpacity(FLinearColor(0.3f, 0.8f, 0.3f)) // 녹색
                    ]
            ]
    
        // 클래스 선택 위젯 행
        + SVerticalBox::Slot()
            .AutoHeight()
            .Padding(0, 2, 0, 8)
            [
                SNew(SHorizontalBox)
    
                    // 업그레이드 전(BaseAbility)
                    + SHorizontalBox::Slot()
                    .FillWidth(1.0f)
                    .Padding(0, 0, 8, 0)
                    [
                        SNew(SClassPropertyEntryBox)
                            .MetaClass(UPGGameplayAbility::StaticClass())
                            .IsBlueprintBaseOnly(true)
                            .AllowAbstract(false)
                            .AllowNone(true)
                            .ClassViewerFilters(ClassFilters)
                            .SelectedClass(this, &FSetBonusDataCustomization::GetSelectedClass, BaseAbilityHandle)
                            .OnSetClass_Lambda([this](const UClass* NewClass)
                                {
                                    if (!BaseAbilityHandle.IsValid() || !BaseAbilityHandle->IsValidHandle()) return;

                                    BaseAbilityHandle->SetValueFromFormattedString(NewClass ? NewClass->GetPathName() : TEXT("None"));
                                }
                            )
                    ]
    
                // 화살표 ( =>)
                + SHorizontalBox::Slot()
                    .AutoWidth()
                    .Padding(4, 0, 4, 0)
                    [
                        SNew(SBox)
                            .WidthOverride(40.f)
                            [
                                SNew(STextBlock)
                                    .Text(FText::FromString(TEXT("⇒")))
                                    .Font(FCoreStyle::GetDefaultFontStyle("Bold", 18))
                                    .ColorAndOpacity(FLinearColor(1.f, 0.85f, 0.f))
                                    .Justification(ETextJustify::Center)
                            ]
                    ]
    
                // 업그레이드 후(UpgradedAbility)
                + SHorizontalBox::Slot()
                    .FillWidth(1.0f)
                    .Padding(8, 0, 0, 0)
                    [
                        SNew(SClassPropertyEntryBox)
                            .MetaClass(UPGGameplayAbility::StaticClass())
                            .IsBlueprintBaseOnly(true)
                            .AllowAbstract(false)
                            .AllowNone(true)
                            .ClassViewerFilters(ClassFilters)
                            .SelectedClass(this, &FSetBonusDataCustomization::GetSelectedClass, UpgradedAbilityHandle)
                            .OnSetClass_Lambda([this](const UClass* NewClass)
                                {
                                    if (!UpgradedAbilityHandle.IsValid() || !UpgradedAbilityHandle->IsValidHandle()) return;

                                    UpgradedAbilityHandle->SetValueFromFormattedString(NewClass ? NewClass->GetPathName() : TEXT("None"));
                                }
                            )
                    ]
            ]
    ];
}

const UClass* FSetBonusDataCustomization::GetSelectedClass(TSharedPtr<IPropertyHandle> Handle) const
{
    if (!Handle.IsValid() || !Handle->IsValidHandle()) return nullptr;

    UObject* Obj = nullptr;
    Handle->GetValue(Obj);

    return Cast<UClass>(Obj);
}