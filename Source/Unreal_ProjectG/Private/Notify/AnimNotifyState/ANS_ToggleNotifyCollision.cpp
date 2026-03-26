// Fill out your copyright notice in the Description page of Project Settings.


#include "Notify/AnimNotifyState/ANS_ToggleNotifyCollision.h"
#include "PGFunctionLibrary.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "PGGameplayTags.h"
#include "Types/PGEnumTypes.h"

void UANS_ToggleNotifyCollision::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
    Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

    // 현재 월드가 게임 월드인지 확인
    UWorld* World = MeshComp->GetWorld();
    if (!World || !World->IsGameWorld())
    {
        return;
    }

    AActor* OwnerActor = MeshComp->GetOwner();
    FGameplayEventData Data;

    // 게임 플레이 이벤트 전송
    UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
        OwnerActor,
        PGGameplayTags::Shared_Event_MeleeTraceStart,
        Data
    );
}

void UANS_ToggleNotifyCollision::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
    UWorld* World = MeshComp->GetWorld();
    if (!World || !World->IsGameWorld())
    {
        return;
    }

    AActor* OwnerActor = MeshComp->GetOwner();
    FGameplayEventData Data;

    // 게임 플레이 이벤트 전송
    UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
        OwnerActor,
        PGGameplayTags::Shared_Event_MeleeTraceEnd,
        Data
    );

    Super::NotifyEnd(MeshComp, Animation, EventReference);
}
