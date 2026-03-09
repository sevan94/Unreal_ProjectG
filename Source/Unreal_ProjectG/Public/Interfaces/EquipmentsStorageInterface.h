// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "EquipmentsStorageInterface.generated.h"

class UEquipmentsStorageComponent;
// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UEquipmentsStorageInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class UNREAL_PROJECTG_API IEquipmentsStorageInterface
{
	GENERATED_BODY()

public:
    virtual UEquipmentsStorageComponent* GetEquipmentsStorageComponent() const = 0;
};
