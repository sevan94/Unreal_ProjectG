// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Lobby/Gacha/GachaActor.h"
#include "Components/SkeletalMeshComponent.h"
#include "NiagaraComponent.h"
#include "TimerManager.h"
#include "Types/PGEnumTypes.h"

// Sets default values
AGachaActor::AGachaActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh")); 
    Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    SetRootComponent(Mesh);

    GachaEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("GachaEffect"));
    GachaEffect->SetupAttachment(RootComponent);
    GachaEffect->bAutoActivate = false;

    MoveTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("MoveTimeline"));

}

void AGachaActor::GachaMove(UUnitUIDataAsset* InData)
{
    switch (InData->UnitRank)
    {
    case(EUnitRank::Normal): GachaColor = FLinearColor::White; break;
    case(EUnitRank::Rare) : GachaColor = FLinearColor::Green; break;
    case(EUnitRank::SuperRare) : GachaColor = FLinearColor::Blue; break;
    default: GachaColor = FLinearColor::White; break;
    }

    if (MoveTimeline)
    {
        MoveTimeline->PlayFromStart();
    }
}

void AGachaActor::GachaOpen(FLinearColor InColor)
{
    // 이미 열렸다면 실행하지 않음
    if (bHasOpened) return;
    bHasOpened = true;

    // 가챠 컬러 변경
    if (GachaEffect)
    {
        GachaEffect->SetVariableLinearColor(TEXT("Color"), InColor);
    }

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
                EndDelegate.BindUObject(this, &AGachaActor::OnGachaeEnded);
                AnimInst->Montage_SetEndDelegate(EndDelegate, OpenMontage);
            }
        }, 0.5f, false);
}

void AGachaActor::GachaReset()
{
    // 타임라인 중지 및 초기화
    if (MoveTimeline)
    {
        MoveTimeline->Stop();
    }

    // 애니메이션 중지 및 메시 초기화
    if (Mesh)
    {
        Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        UAnimInstance* AnimInst = Mesh->GetAnimInstance();
        if (AnimInst)
        {
            // 초기 상태로 되돌림
            AnimInst->Montage_Stop(0.0f);
        }
    }

    // 위치 초기화
    SetActorLocation(StartLocation);

    // 가챠 효과 초기화
    GachaColor = FLinearColor::White;
    if (GachaEffect)
    {
        GachaEffect->Deactivate();
    }

    bHasOpened = false;
}

void AGachaActor::OnGachaeEnded(UAnimMontage* Montage, bool bInterrupted)
{
    // 애니메이션이 정상적으로 끝나면 브로드캐스트
    if (!bInterrupted)
    {
        OnGachaOpenFinished.Broadcast();
    }
}

// Called when the game starts or when spawned
void AGachaActor::BeginPlay()
{
	Super::BeginPlay();
	
    // 타임라인 업데이트 함수 바인딩
    if (MoveCurve)
    {
        // 진행중
        FOnTimelineFloat ProgressFunction;
        ProgressFunction.BindUFunction(this, FName("HandleTimelineProgress"));
        MoveTimeline->AddInterpFloat(MoveCurve, ProgressFunction);

        // 끝난 후
        FOnTimelineEvent FinishedFunction;
        FinishedFunction.BindUFunction(this, FName("HandleTimelineFinished"));
        MoveTimeline->SetTimelineFinishedFunc(FinishedFunction);
    }

    if (Mesh)
    {
        Mesh->OnClicked.AddDynamic(this, &AGachaActor::OnMeshClicked);
        Mesh->OnInputTouchBegin.AddDynamic(this, &AGachaActor::OnMeshTouched);
    }
}

void AGachaActor::OnMeshClicked(UPrimitiveComponent* TouchedComponent, FKey ButtonPressed)
{
    GachaOpen(GachaColor);
}

void AGachaActor::OnMeshTouched(ETouchIndex::Type FingerIndex, UPrimitiveComponent* TouchedComponent)
{
    GachaOpen(GachaColor);
}

void AGachaActor::HandleTimelineProgress(float Value)
{
    FVector NewLocation = FMath::Lerp(StartLocation, EndLocation, Value);
    SetActorLocation(NewLocation);
}

void AGachaActor::HandleTimelineFinished()
{
    if (GachaEffect)
    {
        GachaEffect->SetVariableLinearColor(TEXT("Color"), FLinearColor::White);
        GachaEffect->Activate();
    }
    if (Mesh)
    {
        Mesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    }
}