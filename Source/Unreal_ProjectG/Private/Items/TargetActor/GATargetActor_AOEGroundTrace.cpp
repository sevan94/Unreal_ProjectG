
#include "Items/TargetActor/GATargetActor_AOEGroundTrace.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/DecalComponent.h"
#include "PGFunctionLibrary.h"

AGATargetActor_AOEGroundTrace::AGATargetActor_AOEGroundTrace()
{
    PrimaryActorTick.bCanEverTick = true;
    AOERadiusSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AOERadiusSphere"));
    AOERadiusSphere->SetupAttachment(RootComponent);
    AOERadiusSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    AOERadiusSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
    AOERadiusSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

    AOERadiusSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    AOERadiusSphere->SetGenerateOverlapEvents(true);

    AOEDecalComponent = CreateDefaultSubobject<UDecalComponent>(TEXT("AOEDecalComponent"));
    AOEDecalComponent->SetupAttachment(RootComponent);
    AOEDecalComponent->SetVisibility(false);
}

void AGATargetActor_AOEGroundTrace::StartTargeting(UGameplayAbility* InAbility)
{
    Super::StartTargeting(InAbility);

    // 오버랩 이벤트 바인딩
    AOERadiusSphere->OnComponentBeginOverlap.AddDynamic(this, &AGATargetActor_AOEGroundTrace::OnSphereOverlapBegin);
    AOERadiusSphere->OnComponentEndOverlap.AddDynamic(this, &AGATargetActor_AOEGroundTrace::OnSphereOverlapEnd);

    if (APlayerController* PC = Cast<APlayerController>(PrimaryPC))
    {
        // 터치 이벤트 활성화
        PC->bEnableTouchEvents = true;
    }

    // 데칼 컴포넌트 설정
    AOEDecalComponent->SetDecalMaterial(AOETraceDecalMaterial);
    AOEDecalComponent->SetWorldRotation(AOEDecalRotation);
    AOEDecalComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
    AOEDecalComponent->DecalSize = FVector(10, AOEDecalSize, AOEDecalSize);

    AOERadiusSphere->SetSphereRadius(AOERadius);
}

void AGATargetActor_AOEGroundTrace::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    ClearAllHighlightedActors();
    Super::EndPlay(EndPlayReason);
}

void AGATargetActor_AOEGroundTrace::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    if (!PrimaryPC)
    {
        return;
    }


    FVector2D TouchLocation = FVector2D::ZeroVector;
    bool bIsCurrentlyTouching = false;

    if(APlayerController* PC = Cast<APlayerController>(PrimaryPC))
    {
        PC->GetInputTouchState(ETouchIndex::Touch1, TouchLocation.X, TouchLocation.Y, bIsCurrentlyTouching);
    }

    // 터치 시작 및 종료 감지
    if (bIsCurrentlyTouching && !bIsTouching)
    {
        OnTouchPressed();
    }
    else if (!bIsCurrentlyTouching && bIsTouching)
    {
        OnTouchReleased();
    }

    bIsTouching = bIsCurrentlyTouching;

    if (bIsTouching)
    {
        FVector WorldLocation, WorldDirection;
        if (PrimaryPC->DeprojectScreenPositionToWorld(TouchLocation.X, TouchLocation.Y, WorldLocation, WorldDirection))
        {
            FHitResult HitResult;
            FVector TraceEnd = WorldLocation + (WorldDirection * 10000.0f);

            if (GetWorld()->LineTraceSingleByChannel(HitResult, WorldLocation, TraceEnd, ECC_Visibility))
            {
                LastTouchLocation = HitResult.Location;

                SetActorLocation(LastTouchLocation);
            }
        }
    }

    if (bShowDebugSphere)
    {
        DrawDebugSphere(GetWorld(), LastTouchLocation, AOERadiusSphere->GetScaledSphereRadius(), 12, FColor::Green, false, -1, 0, 2);
    }
}

void AGATargetActor_AOEGroundTrace::OnSphereOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    // 오버랩 팀 확인 및 리턴 추가
    if (!OtherActor)
        return;
    if (OverlappedActors.Contains(OtherActor))
        return;

    // 플레이어 자신의 캐릭터는 무시
    if (PrimaryPC && PrimaryPC->GetPawn() == OtherActor)
        return;

    // 적팀이면 액터 빛나게 하기
    if (UPGFunctionLibrary::IsTargetCharacterIsHostile(OwnerActor, OtherActor))
    {
        // 오버랩된 액터 추가
        OverlappedActors.AddUnique(OtherActor);
        OnHighlightActorInAOE(OtherActor);
    }
}

void AGATargetActor_AOEGroundTrace::OnSphereOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (!OtherActor)
        return;
    if(!OverlappedActors.Contains(OtherActor))
        return;

    // 오버랩된 액터 제거
    OverlappedActors.Remove(OtherActor);
    OnUnhighlightActorOutAOE(OtherActor);
}

void AGATargetActor_AOEGroundTrace::OnTouchPressed()
{
    // 터치 시작 시에 데칼 컴포넌트 생성
    bIsTouching = true;

    // 데칼 컴포넌트 표시
    AOEDecalComponent->SetVisibility(true);
    
    // 스피어 컴포넌트의 반지름을 설정
    AOERadiusSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AGATargetActor_AOEGroundTrace::OnTouchReleased()
{
    bIsTouching = false;

    AOERadiusSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    // 하이라이트된 액터 모두 초기화
    ClearAllHighlightedActors();

    // 현재 오버랩된 액터들을 기반으로 타겟 데이터 생성
    FGameplayAbilityTargetDataHandle TargetDataHandle;

    // 히트 로케이션만 포함하는 타겟 데이터 생성
    FGameplayAbilityTargetDataHandle TargetData;
    FHitResult HitResult;
    HitResult.Location = LastTouchLocation;
    TargetData.Add(new FGameplayAbilityTargetData_SingleTargetHit(HitResult));
    TargetDataReadyDelegate.Broadcast(TargetData);
}

void AGATargetActor_AOEGroundTrace::OnHighlightActorInAOE(AActor* InActor)
{
    // 머티리얼 변경
    TArray<UMeshComponent*> TargetMeshComponents;
    InActor->GetComponents<UMeshComponent>(TargetMeshComponents);

    for (UMeshComponent* MeshComp : TargetMeshComponents)
    {
        if(UMaterialInstanceDynamic* DynamicMat = MeshComp->CreateAndSetMaterialInstanceDynamic(0))
        {
            DynamicMat->SetScalarParameterValue(FName("OverlapFXSwitch"), 1.f);
            HighlightedActorMap.Add(InActor, DynamicMat);
        }
    }
}

void AGATargetActor_AOEGroundTrace::OnUnhighlightActorOutAOE(AActor* InActor)
{
    // 머티리얼 원래대로 복원
    if(UMaterialInstanceDynamic* DynamicMat = *HighlightedActorMap.Find(InActor))
    {
        DynamicMat->SetScalarParameterValue(FName("OverlapFXSwitch"), 0.f);
        HighlightedActorMap.Remove(InActor);
    }
}

void AGATargetActor_AOEGroundTrace::ClearAllHighlightedActors()
{
    for(AActor* Actor : OverlappedActors)
    {
        OnUnhighlightActorOutAOE(Actor);
    }
    OverlappedActors.Empty();
}
