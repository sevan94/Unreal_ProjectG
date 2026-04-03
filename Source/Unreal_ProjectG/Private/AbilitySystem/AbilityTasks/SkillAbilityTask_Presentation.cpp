// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/AbilityTasks/SkillAbilityTask_Presentation.h"

#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Camera/CameraComponent.h"
#include "Character/Hero/HeroCharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "DataAssets/Ability/DataAsset_HeroSkillPresentation.h"
#include "Engine/World.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "PGGameplayTags.h"
#include "TimerManager.h"

USkillAbilityTask_Presentation* USkillAbilityTask_Presentation::Create(UGameplayAbility* OwningAbility, UDataAsset_HeroSkillPresentation* InPresentationData)
{
    if (!OwningAbility || !InPresentationData)
    {
        return nullptr;
    }

    USkillAbilityTask_Presentation* Task = NewAbilityTask<USkillAbilityTask_Presentation>(OwningAbility);
    Task->PresentationData = InPresentationData;
    Task->bTickingTask = true;
    return Task;
}

void USkillAbilityTask_Presentation::Activate()
{
    // Ability와 PresentationData가 유효한지 확인
    if (!Ability || !PresentationData)
    {
        EndTask();
        return;
    }

    AActor* AvatarActor = Ability->GetAvatarActorFromActorInfo();
    if (!AvatarActor)
    {
        EndTask();
        return;
    }

    // HeroCharacter이면 HeroCameraComponent과 SpringArm을 직접 가져오고, 아니면 Actor에서 찾아본다.
    if (AHeroCharacter* HeroCharacter = Cast<AHeroCharacter>(AvatarActor))
    {
        HeroCamera = HeroCharacter->GetHeroCameraComponent();
        SpringArm = HeroCharacter->GetSpringArm();
    }
    else
    {
        HeroCamera = AvatarActor->FindComponentByClass<UCameraComponent>();
        SpringArm = AvatarActor->FindComponentByClass<USpringArmComponent>();
    }

    if (!HeroCamera.IsValid() && !SpringArm.IsValid())
    {
        EndTask();
        return;
    }

    // 기본 글로벌 타임 딜레이션을 캐싱. 히트스톱이 적용될 때 원래 값으로 복원하기 위해 필요.
    if (UWorld* World = GetWorld())
    {
        CachedBaseGlobalTimeDilation = UGameplayStatics::GetGlobalTimeDilation(World);
    }

    // 카메라의 원래 FOV를 저장. 유효하지 않으면 기본값 90을 사용.
    OriginalFOV = HeroCamera.IsValid() ? HeroCamera->FieldOfView : 90.0f;
    if (OriginalFOV <= KINDA_SMALL_NUMBER)
    {
        OriginalFOV = 90.0f;
    }

    // PresentationData에서 줌된 FOV를 가져오고, 5에서 170 사이로 클램프.
    const float ZoomedFOV = FMath::Clamp(PresentationData->ZoomFOVTarget, 5.0f, 170.0f);

    // SpringArm이 유효하면, 원래의 스프링암 설정을 저장하고, 적용할 스프링암 설정을 PresentationData에서 가져온다.
    if (SpringArm.IsValid())
    {
        OriginalSpringArmLength = SpringArm->TargetArmLength;
        OriginalSpringArmTargetOffset = SpringArm->TargetOffset;
        OriginalSpringArmSocketOffset = SpringArm->SocketOffset;
        OriginalSpringArmRotation = SpringArm->GetRelativeRotation();

        ZoomedSpringArmLength = FMath::Max(0.0f, PresentationData->SpringArmLengthTarget);
        ZoomedSpringArmTargetOffset = PresentationData->SpringArmTargetLocation;
        ZoomedSpringArmSocketOffset = PresentationData->SpringArmSocketLocation;
        ZoomedSpringArmRotation = PresentationData->SpringArmRotationTarget;
    }

    // 카메라 연출 시작
    StartZoomTo(ZoomedFOV, PresentationData->ZoomInDuration, false);

    const FGameplayTag ImpactTag = PresentationData->ImpactEventTag.IsValid()
        ? PresentationData->ImpactEventTag
        : PGGameplayTags::Event_Trigger_OnHit;

    // Impact 이벤트를 기다리는 태스크 생성. 이벤트가 발생하면 OnImpactPresentationEvent이 호출된다.
    UAbilityTask_WaitGameplayEvent* WaitImpactEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(Ability, ImpactTag);
    if (WaitImpactEventTask)
    {
        WaitImpactEventTask->EventReceived.AddDynamic(this, &USkillAbilityTask_Presentation::OnImpactPresentationEvent);
        WaitImpactEventTask->ReadyForActivation();
    }
}

void USkillAbilityTask_Presentation::TickTask(float DeltaTime)
{
    Super::TickTask(DeltaTime);

    if (!bLerpActive)
    {
        return;
    }

    // Lerp 진행, 더 부드러운 카메라 연출을 위해 EaseInOut 보간 사용
    LerpElapsed += DeltaTime;

    const float RawAlpha = (LerpDuration > KINDA_SMALL_NUMBER)
        ? FMath::Clamp(LerpElapsed / LerpDuration, 0.0f, 1.0f)
        : 1.0f;
    const float SmoothAlpha = FMath::InterpEaseInOut(0.0f, 1.0f, RawAlpha, 2.0f);

    if (HeroCamera.IsValid())
    {
        HeroCamera->SetFieldOfView(FMath::Lerp(LerpStartFOV, LerpTargetFOV, SmoothAlpha));
    }

    if (SpringArm.IsValid())
    {
        SpringArm->TargetArmLength = FMath::Lerp(LerpStartSpringArmLength, LerpTargetSpringArmLength, SmoothAlpha);
        SpringArm->TargetOffset = FMath::Lerp(LerpStartSpringArmTargetOffset, LerpTargetSpringArmTargetOffset, SmoothAlpha);
        SpringArm->SocketOffset = FMath::Lerp(LerpStartSpringArmSocketOffset, LerpTargetSpringArmSocketOffset, SmoothAlpha);
        SpringArm->SetRelativeRotation(FMath::Lerp(LerpStartSpringArmRotation, LerpTargetSpringArmRotation, SmoothAlpha));
    }

    // 알파가 1에 도달하면 타깃에 완전히 도달한 것이므로 Lerp를 종료한다.
    if (RawAlpha >= 1.0f)
    {
        bLerpActive = false;

        if (bEndTaskWhenLerpDone)
        {
            EndTask();
        }
    }
}

void USkillAbilityTask_Presentation::OnDestroy(bool bInOwnerFinished)
{
    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().ClearTimer(HitStopTimerHandle);
    }

    if (bHitStopActive)
    {
        if (UWorld* World = GetWorld())
        {
            UGameplayStatics::SetGlobalTimeDilation(World, CachedBaseGlobalTimeDilation);
        }
        bHitStopActive = false;
    }

    // 혹시 카메라가 원상 복구가 안된 상태로 남아있을 수 있으니, 확실히 원상 복구한다.
    RestoreImmediate();

    Super::OnDestroy(bInOwnerFinished);
}

void USkillAbilityTask_Presentation::ForceRestorAndEnd()
{
    if (bHitStopActive)
    {
        if (UWorld* World = GetWorld())
        {
            World->GetTimerManager().ClearTimer(HitStopTimerHandle);
            UGameplayStatics::SetGlobalTimeDilation(World, CachedBaseGlobalTimeDilation);
        }
        bHitStopActive = false;
    }

    RestoreImmediate();
    EndTask();
}

void USkillAbilityTask_Presentation::OnImpactPresentationEvent(FGameplayEventData Payload)
{
    (void)Payload;

    if (bImpactHandled)
    {
        return;
    }

    bImpactHandled = true;  // 이벤트가 들어왔음.
    ApplyHitStopAndHighlight();
}

void USkillAbilityTask_Presentation::StartZoomTo(float InTargetFOV, float InDuration, bool bEndWhenDone)
{
    bEndTaskWhenLerpDone = bEndWhenDone;                                         // Lerp가 끝났을 때 태스크를 종료할지 여부 설정, 즉, 연충 시작이냐 연출 종료냐를 구분하는 플래그
    LerpElapsed = 0.0f;                                                          // 현재 시점을 0으로 초기화
    LerpDuration = FMath::Max(0.0f, InDuration);
    LerpStartFOV = HeroCamera.IsValid() ? HeroCamera->FieldOfView : OriginalFOV; // 현재 FOV에서 시작(연출이 시작 전이든 후든 상관없이 현재 카메라의 FOV를 가져옴
    LerpTargetFOV = InTargetFOV;

    const bool bRestoreToOriginal = FMath::IsNearlyEqual(InTargetFOV, OriginalFOV, KINDA_SMALL_NUMBER); // 타깃 FOV가 원래 FOV와 거의 같은지 여부 계산, 즉, 연출이 끝나고 원래 상태로 돌아가는 연출인지 여부(연출 끝이냐? 연출 시작이냐? 구분하는 플래그)

    if (SpringArm.IsValid())
    {
        LerpStartSpringArmLength = SpringArm->TargetArmLength;
        LerpStartSpringArmTargetOffset = SpringArm->TargetOffset;
        LerpStartSpringArmSocketOffset = SpringArm->SocketOffset;
        LerpStartSpringArmRotation = SpringArm->GetRelativeRotation();

        if (bRestoreToOriginal)
        {
            // 타깃 FOV가 원래 FOV와 거의 같다면, 스프링암도 원래 상태로 돌아가는 연출이므로 타깃 스프링암 설정을 원래 설정으로 한다.
            LerpTargetSpringArmLength = OriginalSpringArmLength;
            LerpTargetSpringArmTargetOffset = OriginalSpringArmTargetOffset;
            LerpTargetSpringArmSocketOffset = OriginalSpringArmSocketOffset;
            LerpTargetSpringArmRotation = OriginalSpringArmRotation;
        }
        else
        {
            // 타깃 FOV가 원래 FOV와 다르다면, 스프링암도 줌된 상태로 가는 연출이므로 타깃 스프링암 설정을 줌된 설정으로 한다.
            LerpTargetSpringArmLength = ZoomedSpringArmLength;
            LerpTargetSpringArmTargetOffset = ZoomedSpringArmTargetOffset;
            LerpTargetSpringArmSocketOffset = ZoomedSpringArmSocketOffset;
            LerpTargetSpringArmRotation = ZoomedSpringArmRotation;
        }
    }

    bLerpActive = true;

    // Duration이 거의 0(0.0001)이라면 Lerp를 건너뛰고 바로 타깃 설정으로 이동한다.
    if (LerpDuration <= KINDA_SMALL_NUMBER)
    {
        if (HeroCamera.IsValid())
        {
            HeroCamera->SetFieldOfView(LerpTargetFOV);
        }

        if (SpringArm.IsValid())
        {
            SpringArm->TargetArmLength = LerpTargetSpringArmLength;
            SpringArm->TargetOffset = LerpTargetSpringArmTargetOffset;
            SpringArm->SocketOffset = LerpTargetSpringArmSocketOffset;
            SpringArm->SetRelativeRotation(LerpTargetSpringArmRotation);
        }

        bLerpActive = false;

        if (bEndTaskWhenLerpDone)
        {
            EndTask();
        }
    }
}

void USkillAbilityTask_Presentation::ApplyHitStopAndHighlight()
{
    if (!Ability || !PresentationData)
    {
        EndTask();
        return;
    }

    // 나이아가라가 있으면 아바타 액터에 붙여서 재생
    if (AActor* AvatarActor = Ability->GetAvatarActorFromActorInfo())
    {
        if (PresentationData->ImpactNiagara)
        {
            USceneComponent* AttachComponent = AvatarActor->GetRootComponent();
            FName SocketName = NAME_None;

            if (ACharacter* AvatarCharacter = Cast<ACharacter>(AvatarActor))
            {
                if (USkeletalMeshComponent* MeshComponent = AvatarCharacter->GetMesh())
                {
                    AttachComponent = MeshComponent;

                    if (PresentationData->ImpactNiagaraSocketName != NAME_None &&
                        MeshComponent->DoesSocketExist(PresentationData->ImpactNiagaraSocketName))
                    {
                        SocketName = PresentationData->ImpactNiagaraSocketName;
                    }
                }
            }

            if (AttachComponent)
            {
                UNiagaraFunctionLibrary::SpawnSystemAttached(
                    PresentationData->ImpactNiagara,
                    AttachComponent,
                    SocketName,
                    FVector::ZeroVector,
                    FRotator::ZeroRotator,
                    EAttachLocation::SnapToTargetIncludingScale,
                    true);
            }
        }
    }

    // 딜레이션 설정
    const float HitStopDuration = FMath::Max(0.0f, PresentationData->HitStopDuration);
    if (HitStopDuration <= KINDA_SMALL_NUMBER)
    {
        EndHitStop();
        return;
    }

    UWorld* World = GetWorld();
    if (!World)
    {
        EndHitStop();
        return;
    }

    // 딜레이션 적용
    const float HitStopScale = FMath::Clamp(PresentationData->HitStopDilation, 0.01f, 1.0f);
    const float NewGlobalTimeDilation = FMath::Max(KINDA_SMALL_NUMBER, CachedBaseGlobalTimeDilation * HitStopScale);

    UGameplayStatics::SetGlobalTimeDilation(World, NewGlobalTimeDilation);
    bHitStopActive = true;

    // 딜레이션이 적용된 상태에서의 딜레이션 해제까지의 기간은 딜레이션이 적용된 시간 * 딜레이션 스케일. 
    const float GameDurationForRealSeconds = FMath::Max(KINDA_SMALL_NUMBER, HitStopDuration * NewGlobalTimeDilation);
    World->GetTimerManager().SetTimer(HitStopTimerHandle, this, &USkillAbilityTask_Presentation::EndHitStop, GameDurationForRealSeconds, false);
}

void USkillAbilityTask_Presentation::EndHitStop()
{
    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().ClearTimer(HitStopTimerHandle);
    }

    // 히트스톱이 활성화된 상태라면 글로벌 타임 딜레이션을 원래 값으로 복원하고, 히트스톱 활성화 플래그를 끈다.
    if (bHitStopActive)
    {
        if (UWorld* World = GetWorld())
        {
            UGameplayStatics::SetGlobalTimeDilation(World, CachedBaseGlobalTimeDilation);
        }
        bHitStopActive = false;
    }

    // 카메라 쉐이크가 있다면 카메라 쉐이크 재생
    if (PresentationData && PresentationData->CameraShake && Ability)
    {
        if (AActor* AvatarActor = Ability->GetAvatarActorFromActorInfo())
        {
            UGameplayStatics::PlayWorldCameraShake(
                AvatarActor,
                PresentationData->CameraShake,
                AvatarActor->GetActorLocation(),
                0.0f,
                3000.0f);
        }
    }

    // 카메라를 원래 FOV로 되돌리는 Lerp 시작. Lerp가 끝나면 태스크 종료.
    StartZoomTo(OriginalFOV, PresentationData ? PresentationData->ZoomOutDuration : 0.0f, true);
}

void USkillAbilityTask_Presentation::RestoreImmediate()
{
    if (HeroCamera.IsValid())
    {
        HeroCamera->SetFieldOfView(OriginalFOV);
    }

    if (SpringArm.IsValid())
    {
        SpringArm->TargetArmLength = OriginalSpringArmLength;
        SpringArm->TargetOffset = OriginalSpringArmTargetOffset;
        SpringArm->SocketOffset = OriginalSpringArmSocketOffset;
        SpringArm->SetRelativeRotation(OriginalSpringArmRotation);
    }

    bLerpActive = false;
    bEndTaskWhenLerpDone = false;
}
