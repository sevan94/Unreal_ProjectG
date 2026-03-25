// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EquipGachaActor.generated.h"

class UNiagaraComponent;
class USkeletalMeshComponent;
class UAnimMontage;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnChestOpenFinished);
UCLASS()
class UNREAL_PROJECTG_API AEquipGachaActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEquipGachaActor();
    
    void ChestOpen();

    UFUNCTION(BlueprintCallable, Category = "Gacha|Actions")
    void ChestReset();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

    // 몽타주 종료를 감지하기 위한 함수
    void OnChestOpen(UAnimMontage* Montage, bool bInterrupted);

public:
    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnChestOpenFinished OnChestOpenFinished;

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gacha")
    TObjectPtr<USkeletalMeshComponent> Mesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gacha")
    TObjectPtr<UNiagaraComponent> GachaEffect;

    UPROPERTY(EditAnywhere, Category = "Gacha|Settings")
    TObjectPtr<UAnimMontage> OpenMontage;
};
