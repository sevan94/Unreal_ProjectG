// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"
#include "DataAssets/UI/UnitUIDataAsset.h"
#include "GachaActor.generated.h"

class UNiagaraComponent;
class USkeletalMeshComponent;
class UAnimMontage;


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGachaOpenFinished);
UCLASS()
class UNREAL_PROJECTG_API AGachaActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGachaActor();

    UFUNCTION(BlueprintCallable, Category = "Gacha|Actions")
    void GachaMove(UUnitUIDataAsset* InData);

    UFUNCTION(BlueprintCallable, Category = "Gacha|Actions")
    void GachaOpen(FLinearColor InColor);

    UFUNCTION(BlueprintCallable, Category = "Gacha|Actions")
    void GachaReset();

    // 몽타주 종료를 감지하기 위한 함수
    void OnGachaeEnded(UAnimMontage* Montage, bool bInterrupted);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

    // 연출 화면에서 입력을 받을 함수
    UFUNCTION()
    void OnMeshClicked(UPrimitiveComponent* TouchedComponent, FKey ButtonPressed);
    UFUNCTION()
    void OnMeshTouched(ETouchIndex::Type FingerIndex, UPrimitiveComponent* TouchedComponent);

private:
    // 타임라인 업데이트용 함수
    UFUNCTION()
    void HandleTimelineProgress(float Value);

    UFUNCTION()
    void HandleTimelineFinished();

public:
    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnGachaOpenFinished OnGachaOpenFinished;

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gacha")
    TObjectPtr<USkeletalMeshComponent> Mesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gacha")
    TObjectPtr<UNiagaraComponent> GachaEffect;

    UPROPERTY(VisibleAnywhere, Category = "Gacha")
    TObjectPtr<UTimelineComponent> MoveTimeline;

    UPROPERTY(EditAnywhere, Category = "Gacha|Settings")
    TObjectPtr<UCurveFloat> MoveCurve;

    UPROPERTY(EditAnywhere, Category = "Gacha|Settings")
    TObjectPtr<UAnimMontage> OpenMontage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gacha|Settings")
    FVector StartLocation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gacha|Settings")
    FVector EndLocation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gacha|Settings")
    FLinearColor GachaColor = FLinearColor::Blue;

private:
    bool bHasOpened = false;
};
