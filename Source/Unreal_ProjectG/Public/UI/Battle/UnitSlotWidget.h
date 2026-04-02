// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UnitSlotWidget.generated.h"

class UUnitUIDataAsset;
class UButton;
class UImage;
class UTextBlock;
class ABaseStructure;

/**
 * 
 */
UCLASS()
class UNREAL_PROJECTG_API UUnitSlotWidget : public UUserWidget
{
	GENERATED_BODY()

public:
    // 데이터 에셋을 가져와서 초기화
    UFUNCTION(BlueprintCallable, Category = "BattelUI")
    void InitializeSlot(UUnitUIDataAsset* InDataAsset);

    void UpdateSlot(float InCost);

    void SetSpawnBase(ABaseStructure* InSpawnBase) {SpawnBase = InSpawnBase;}

    bool IsSpawnAble() const;

    void ExecuteSpawn();

protected:
    // 초기화 및 버튼 이벤트 바인딩
    virtual void NativeConstruct() override;

    // 버튼 클릭 시 실행될 함수
    UFUNCTION()
    void OnUnitButtonClicked();

private:
    UFUNCTION()
    void ResetSpawnCooldown();

protected:
    UPROPERTY(BlueprintReadOnly, Category = "BattelUI")
    TObjectPtr<UUnitUIDataAsset> UnitData;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> UnitButton;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UImage> UnitImage;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> UnitCost;

private:
    TObjectPtr<ABaseStructure> SpawnBase;
    FVector SpawnLocation = FVector(0.0f, 0.0f, 100.0f);

    bool bIsSpawnCooldown = false;

    FTimerHandle SpawnCooldownTimerHandle;


};
