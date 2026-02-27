// Fill out your copyright notice in the Description page of Project Settings.


#include "PGEditor.h"
#include "PropertyEditorModule.h"
#include "Customization/AbilityEntryCustomization.h"
#include "Customization/SetBonusDataCustomization.h"
#include "Types/PGStructTypes.h"
#include "DataAssets/Items/DataAsset_SetBonusData.h"

IMPLEMENT_MODULE(FPGEditorModule, PGEditor)

void FPGEditorModule::StartupModule()
{
    FPropertyEditorModule& PropertyModule =
        FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");

    // FAbilityEntry 커스터마이저 등록
    // FAbilityEntry를 디테일 패널에서 사용할때, FAbilityEntryCustomization을 사용하여 커스터마이징
    // RegisterCustomPropertyTypeLayout 함수 내부는 FAbilityEntryCustomization::MakeInstance 함수를 콜백으로 이용하도록 되어있음
    PropertyModule.RegisterCustomPropertyTypeLayout(
        FAbilityEntry::StaticStruct()->GetFName(),
        FOnGetPropertyTypeCustomizationInstance::CreateStatic(
            &FAbilityEntryCustomization::MakeInstance));

    // UDataAssets_SetBonusData 디테일 커스터 마이저 등록
    PropertyModule.RegisterCustomClassLayout(
        UDataAsset_SetBonusData::StaticClass()->GetFName(),
        FOnGetDetailCustomizationInstance::CreateStatic(
            &FSetBonusDataCustomization::MakeInstance));

    PropertyModule.NotifyCustomizationModuleChanged();
}

void FPGEditorModule::ShutdownModule()
{
    if (FModuleManager::Get().IsModuleLoaded("PropertyEditor"))
    {
        FPropertyEditorModule& PropertyModule = 
            FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");

        // FAbilityEntry 커스터마이저 등록 해제
        PropertyModule.UnregisterCustomPropertyTypeLayout(
            FAbilityEntry::StaticStruct()->GetFName());

        // UDataAssets_SetBonusData 디테일 커스터마이저 등록 해제
        PropertyModule.UnregisterCustomClassLayout(
            UDataAsset_SetBonusData::StaticClass()->GetFName());
    }
}
