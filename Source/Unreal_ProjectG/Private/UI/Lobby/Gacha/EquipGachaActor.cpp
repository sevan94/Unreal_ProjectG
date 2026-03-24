// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Lobby/Gacha/EquipGachaActor.h"
#include "Components/SkeletalMeshComponent.h"
#include "NiagaraComponent.h"

// Sets default values
AEquipGachaActor::AEquipGachaActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
    SetRootComponent(Mesh);

    GachaEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("GachaEffect"));
    GachaEffect->SetupAttachment(RootComponent);
    GachaEffect->bAutoActivate = false;
}

void AEquipGachaActor::ChestOpen()
{
    GachaEffect->Activate();
    // 2. 0.5초 후 Play Montage
    FTimerHandle OpenTimerHandle;
    GetWorldTimerManager().SetTimer(OpenTimerHandle, [this]()
        {
            if (Mesh && OpenMontage)
            {
                UAnimInstance* AnimInst = Mesh->GetAnimInstance();
                AnimInst->Montage_Play(OpenMontage);

                // 몽타주가 끝나는 시점에 실행될 바인딩
                FOnMontageEnded EndDelegate;
                EndDelegate.BindUObject(this, &AEquipGachaActor::OnChestOpen);
                AnimInst->Montage_SetEndDelegate(EndDelegate, OpenMontage);
            }
        }, 0.5f, false);
}

// Called when the game starts or when spawned
void AEquipGachaActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AEquipGachaActor::OnChestOpen(UAnimMontage* Montage, bool bInterrupted)
{
    // 애니메이션이 끝나면 브로드캐스트
    OnChestOpenFinished.Broadcast();
}

void AEquipGachaActor::ChestReset()
{
    if (GachaEffect)
    {
        GachaEffect->Deactivate();
    }
}

