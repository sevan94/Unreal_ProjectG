// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SetEffectWidget.generated.h"

class UImage;
class UTextBlock;

/**
 * 
 */
UCLASS()
class UNREAL_PROJECTG_API USetEffectWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
    // 데이터 테이블의 정보를 바탕으로 위젯 UI를 갱신하는 함수
    UFUNCTION(BlueprintCallable, Category = "UI")
    void UpdateSetDisplay(UTexture2D* InIcon, FText InName, FText InDescription);

protected:
    // 스크린샷의 계층 구조와 이름을 일치시켜 바인딩합니다.
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UImage> SetImage;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> SetName;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> SetDescription;
};
