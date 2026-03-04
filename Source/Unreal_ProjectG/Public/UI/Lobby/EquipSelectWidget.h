// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "EquipSelectWidget.generated.h"

class UImage;
class UButton;
class UOverlay;
class UEquipEntryObject;
/**
 * 
 */
UCLASS()
class UNREAL_PROJECTG_API UEquipSelectWidget : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

protected:
    // IUserObjectListEntry 인터페이스 구현 (데이터가 설정될 때 호출됨)
    virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;

    UFUNCTION()
    void OnEquipClicked();

    // 위젯 내부 데이터 갱신
    void UpdateWidget(UEquipEntryObject* InEntryObject);

protected:
    // 장비 이미지
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UImage> EquipImage;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> EquipButton;

    // 잠금 표시용 오버레이
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UOverlay> LockOverlay;
};
