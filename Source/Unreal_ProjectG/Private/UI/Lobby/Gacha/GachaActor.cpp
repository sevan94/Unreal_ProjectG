// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Lobby/Gacha/GachaActor.h"
#include "Components/SkeletalMeshComponent.h"
#include "NiagaraComponent.h"
#include "TimerManager.h"

// Sets default values
AGachaActor::AGachaActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
    SetRootComponent(Mesh);

    GachaEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("GachaEffect"));
    GachaEffect->SetupAttachment(RootComponent);
    GachaEffect->bAutoActivate = false;

    MoveTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("MoveTimeline"));

}

void AGachaActor::GachaMove()
{
    GachaColor = FLinearColor::White;
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
                Mesh->GetAnimInstance()->Montage_Play(OpenMontage);
            }
        }, 0.5f, false);
}

void AGachaActor::GachaReset()
{
    // 위치 초기화
    SetActorLocation(StartLocation);

    // 가챠 효과 초기화
    if (GachaEffect)
    {
        GachaEffect->Deactivate();
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
}