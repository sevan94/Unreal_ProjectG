// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Shared/SharedAbility_BuffAura.h"
#include "AbilitySystemComponent.h"
#include "Components/SphereComponent.h"
#include "Components/DecalComponent.h"
#include "PGFunctionLibrary.h"
#include "Character/PGCharacterBase.h"
#include "DataAssets/Ability/DataAsset_SkillData.h"
#include "PGGameplayTags.h"

void USharedAbility_BuffAura::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
    Super::OnGiveAbility(ActorInfo, Spec);

    UDataAsset_SkillData* DataAsset = Cast<UDataAsset_SkillData>(GetCurrentAbilitySpec()->SourceObject.Get());
    if (DataAsset)
    {
        const FAbilityEntry& SelectedAbilityEntry = DataAsset->GetGivenAbilityEntryForASC(GetAbilitySystemComponentFromActorInfo());
        const FSharedBuffAuraAbilityConfig* Config = SelectedAbilityEntry.AbilityConfig.GetPtr<FSharedBuffAuraAbilityConfig>();
        if (Config)
        {
            BuffAuraConfig = *Config;
        }
    }
}

void USharedAbility_BuffAura::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    //==============================================
    // FSharedBuffAuraAbilityConfig의 SoftPtr 로드
    BuffAuraConfig.AuraRadiusDecalMaterial.LoadSynchronous();
    //==============================================

    BuildCachedBuffEffectSpecs();

    BuffAuraSphere = NewObject<USphereComponent>(GetAvatarActorFromActorInfo());
    BuffAuraSphere->SetupAttachment(GetAvatarActorFromActorInfo()->GetRootComponent());
    BuffAuraSphere->SetSphereRadius(BuffAuraConfig.BuffAuraRadius);
    BuffAuraSphere->RegisterComponent(); // 컴포넌트 등록
    BuffAuraSphere->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));
    BuffAuraSphere->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);

    // 오버랩 이벤트 바인딩
    BuffAuraSphere->OnComponentBeginOverlap.AddUniqueDynamic(this, &USharedAbility_BuffAura::OnAuraBeginOverlap);
    BuffAuraSphere->OnComponentEndOverlap.AddUniqueDynamic(this, &USharedAbility_BuffAura::OnAuraEndOverlap);

    // 오라 범위를 시각적으로 보여주는 데칼 생성
    if (BuffAuraConfig.AuraRadiusDecalMaterial.IsValid())
    {
        BuffAuraDecal = NewObject<UDecalComponent>(GetAvatarActorFromActorInfo());
        BuffAuraDecal->SetupAttachment(GetAvatarActorFromActorInfo()->GetRootComponent());
        BuffAuraDecal->SetDecalMaterial(BuffAuraConfig.AuraRadiusDecalMaterial.Get());
        BuffAuraDecal->DecalSize = FVector(200.f, BuffAuraConfig.BuffAuraRadius, BuffAuraConfig.BuffAuraRadius);
        BuffAuraDecal->SetRelativeRotation(FRotator(-90.f, 0.f, 0.f));
        BuffAuraDecal->RegisterComponent();
    }
}

void USharedAbility_BuffAura::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
    // 버프 오라가 적용된 액터들에게 버프 제거 후 정리
    if (ActiveBuffsOnTargets.Num() > 0)
    {
        for (const auto& Elem : ActiveBuffsOnTargets)
        {
            const TArray<FActiveGameplayEffectHandle>& Handles = Elem.Value;
            for (const FActiveGameplayEffectHandle& EffectHandle : Handles)
            {
                if (Elem.Key.IsValid())
                {
                    NativeRemoveActiveGameplayEffectFromTarget(Elem.Key.Get(), EffectHandle);

                }
            }
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
    if (UPGFunctionLibrary::IsTargetCharacterHostile(GetAvatarActorFromActorInfo(), OtherActor)) return;
    if (!Cast<APGCharacterBase>(OtherActor)) return;

    ApplyBuffAuraEffectToTarget(OtherActor);
}

void USharedAbility_BuffAura::OnAuraEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    TWeakObjectPtr<AActor> Key = OtherActor;

    if (ActiveBuffsOnTargets.Contains(Key.Get()))
    {
        if (TArray<FActiveGameplayEffectHandle>* Handles = ActiveBuffsOnTargets.Find(Key))
        {
            for(const FActiveGameplayEffectHandle& EffectHandle : *Handles)
            {
                NativeRemoveActiveGameplayEffectFromTarget(OtherActor, EffectHandle);
            }
            ActiveBuffsOnTargets.Remove(Key);
        }
    }
}

void USharedAbility_BuffAura::BuildCachedBuffEffectSpecs()
{
    CachedNumericBuffSpecs.Empty();
    CachedStatusBuffSpecs.Empty();

    //수치형 버프
    for (const FBuffEffectConfig& Buff : BuffAuraConfig.Buffs)
    {
        if (Buff.BuffEffectClass)
        {
            FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(Buff.BuffEffectClass.Get(), GetAbilityLevel());
            
            // 버프 효과의 magnitude를 설정
            if (SpecHandle.IsValid())
            {
                const float Multiplier = Buff.SkillMultiplier.GetValueAtLevel(GetAbilityLevel());
                const float BaseAmount = Buff.BaseBuffAmount.GetValueAtLevel(GetAbilityLevel());

                SpecHandle.Data->SetSetByCallerMagnitude(PGGameplayTags::Shared_SetByCaller_SkillMultiplier, Multiplier);
                SpecHandle.Data->SetSetByCallerMagnitude(PGGameplayTags::Shared_SetByCaller_BaseAmount, BaseAmount);
            }
            CachedNumericBuffSpecs.Add(SpecHandle);
        }
    }
}

void USharedAbility_BuffAura::ApplyBuffAuraEffectToTarget(AActor* TargetActor)
{
    if (!TargetActor) return;

    TArray<FActiveGameplayEffectHandle> AppliedHandles;

    // 수치형 버프 적용
    for(const FGameplayEffectSpecHandle& SpecHandle : CachedNumericBuffSpecs)
    {
        FActiveGameplayEffectHandle ActiveEffectHandle = NativeApplyEffectSpecHandleToTarget(TargetActor, SpecHandle);
        if (ActiveEffectHandle.IsValid())
        {
            AppliedHandles.Add(ActiveEffectHandle);
        }
    }

    // 상태형 버프 적용
    for (const FGameplayEffectSpecHandle& SpecHandle : CachedStatusBuffSpecs)
    {
        FActiveGameplayEffectHandle Handle = NativeApplyEffectSpecHandleToTarget(TargetActor, SpecHandle);
        if(Handle.IsValid())
        {
            AppliedHandles.Add(Handle);
        }
    }

    if(AppliedHandles.Num() > 0)
    {
        // 적용된 버프 핸들을 ActiveBuffsOnTargets에 저장
        ActiveBuffsOnTargets.Add(TargetActor, AppliedHandles);
    }
}


