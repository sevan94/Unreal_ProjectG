// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "IDetailCustomization.h"
#include "IPropertyUtilities.h"
#include "PropertyHandle.h"

/**
 *  UDataAsset_SetBonusData의 커스터마이저
 */
class FSetBonusDataCustomization : public IDetailCustomization
{
public:
    static TSharedRef<IDetailCustomization> MakeInstance()
    {
        return MakeShareable(new FSetBonusDataCustomization());
    }

    // UI구조를 재구성하는 함수
    virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override;

private:

    const UClass* GetSelectedClass(TSharedPtr<IPropertyHandle> Handle) const;

    // 프로퍼티 핸들
    // 특정 UPROPERTY 변수에 접근하기 위한 핸들, 이를 통해 해당 변수의 값을 읽거나 쓸 수 있음
    TSharedPtr<IPropertyHandle> BaseAbilityHandle;
    TSharedPtr<IPropertyHandle> UpgradedAbilityHandle;
    TSharedPtr<IPropertyHandle> SetTagHandle;

    // 패널 업데이트 용
    TSharedPtr<IPropertyUtilities> PropertyUtilities; // 디테일 패널과의 통신을 위한 유틸리티, 강제 새로 고침, 선택된 객체 가져오기 등
};
