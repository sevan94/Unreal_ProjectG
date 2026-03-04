// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Shared/SharedAbility_BuffAura.h"
#include "AbilitySystemComponent.h"
#include "Components/SphereComponent.h"
#include "Components/DecalComponent.h"
#include "PGFunctionLibrary.h"
#include "Character/PGCharacterBase.h"

void USharedAbility_BuffAura::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    BuildCachedBuffEffectSpecs();

    BuffAuraSphere = NewObject<USphereComponent>(GetAvatarActorFromActorInfo());
    BuffAuraSphere->SetupAttachment(GetAvatarActorFromActorInfo()->GetRootComponent());
    BuffAuraSphere->SetSphereRadius(BuffAuraRadius);
    BuffAuraSphere->RegisterComponent(); // 컴포넌트 등록
    BuffAuraSphere->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));
    BuffAuraSphere->SetCollisionResponseToAllChannels(ECR_Ignore);

    // 오버랩 이벤트 바인딩
    BuffAuraSphere->OnComponentBeginOverlap.AddUniqueDynamic(this, &USharedAbility_BuffAura::OnAuraBeginOverlap);
    BuffAuraSphere->OnComponentEndOverlap.AddUniqueDynamic(this, &USharedAbility_BuffAura::OnAuraEndOverlap);

    // 오라 범위를 시각적으로 보여주는 데칼 생성
    if (AuraRadiusDecalMaterial)
    {
        BuffAuraDecal = NewObject<UDecalComponent>(GetAvatarActorFromActorInfo());
        BuffAuraDecal->SetupAttachment(GetAvatarActorFromActorInfo()->GetRootComponent());
        BuffAuraDecal->SetDecalMaterial(AuraRadiusDecalMaterial);
        BuffAuraDecal->DecalSize = FVector(200.f, BuffAuraRadius, BuffAuraRadius);
        BuffAuraDecal->SetRelativeRotation(FRotator(-90.f, 0.f, 0.f));
        BuffAuraDecal->RegisterComponent();
    }
}

void USharedAbility_BuffAura::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
    // 버프 오라가 적용된 액터들에게 버프 제거 후 정리
    if (ActiveBuffsOnTargets.Num() > 0)
    {
        for(const auto& Elem : ActiveBuffsOnTargets)
        {
            AActor* TargetActor = Elem.Key;
            FActiveGameplayEffectHandle EffectHandle = Elem.Value;
            NativeRemoveActiveGameplayEffectFromTarget(TargetActor, EffectHandle);
        }
        ActiveBuffsOnTargets.Empty();
    }

    // 버프 오라 컴포넌트 제거
    if (BuffAuraSphere)
    {
        BuffAuraSphere->DestroyComponent();
        BuffAuraSphere = nullptr;
    }

    // 데칼 컴포넌트 제거
    if(BuffAuraDecal)
    {
        BuffAuraDecal->DestroyComponent();
        BuffAuraDecal = nullptr;
    }

    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void USharedAbility_BuffAura::OnAuraBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    // 이미 버프가 적용된 액터이거나, 적대적인 대상이 아니라면 무시
    if (ActiveBuffsOnTargets.Contains(OtherActor)) return;
    //if(OtherActor<APGCharacterBase>(OtherActor))
    if (UPGFunctionLibrary::IsTargetCharacterHostile(GetAvatarActorFromActorInfo(), OtherActor)) return;
    
    ApplyBuffAuraEffectToTarget(OtherActor);
}

void USharedAbility_BuffAura::OnAuraEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (ActiveBuffsOnTargets.Contains(OtherActor))
    {
        // ActiveBuffsOnTargets에 해당 액터가 존재하면 모두 제거
        FActiveGameplayEffectHandle& EffectHandle = ActiveBuffsOnTargets[OtherActor];
        NativeRemoveActiveGameplayEffectFromTarget(OtherActor, EffectHandle);
        ActiveBuffsOnTargets.Remove(OtherActor);
    }
}

void USharedAbility_BuffAura::BuildCachedBuffEffectSpecs()
{
    CachedBuffEffectSpecs.Empty();
    for (const TSubclassOf<UGameplayEffect>& EffectClass : BuffAuraEffectClasses)
    {
        if (EffectClass)
        {
            FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(EffectClass, GetAbilityLevel());
            CachedBuffEffectSpecs.Add(SpecHandle);
        }
    }
}

void USharedAbility_BuffAura::ApplyBuffAuraEffectToTarget(AActor* TargetActor)
{
    if (TargetActor && CachedBuffEffectSpecs.Num() > 0)
    {
        for (const FGameplayEffectSpecHandle& SpecHandle : CachedBuffEffectSpecs)
        {
            FActiveGameplayEffectHandle ActiveEffectHandle = NativeApplyEffectSpecHandleToTarget(TargetActor, SpecHandle);
            if (ActiveEffectHandle.IsValid())
            {
                ActiveBuffsOnTargets.Add(TargetActor, ActiveEffectHandle);
            }
        }
    }
}


