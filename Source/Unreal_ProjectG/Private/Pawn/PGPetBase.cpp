// Fill out your copyright notice in the Description page of Project Settings.


#include "Pawn/PGPetBase.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "AbilitySystem/PGAbilitySystemComponent.h"

APGPetBase::APGPetBase()
{
	PrimaryActorTick.bCanEverTick = false;

    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
    // SkeletalMeshComponent 생성 및 설정
    PetSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("PetSkeletalMesh"));
    PetSkeletalMesh->SetupAttachment(RootComponent);

    // FloatingPawnMovement 생성 및 설정
    PetMovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("PetMovementComponent"));

    // AbilitySystemComponent 생성 및 설정
    PGAbilitySystemComponent = CreateDefaultSubobject<UPGAbilitySystemComponent>(TEXT("PGAbilitySystemComponent"));
}

void APGPetBase::BeginPlay()
{
	Super::BeginPlay();


}

UAbilitySystemComponent* APGPetBase::GetAbilitySystemComponent() const
{
    return nullptr;
}
