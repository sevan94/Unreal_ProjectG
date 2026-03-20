#include "Actors/SkillActor.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "NiagaraComponent.h"
#include "Components/AudioComponent.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "DataAssets/Ability/DataAsset_SkillVisualData.h"
#include "PGGameplayTags.h"
#include "PGFunctionLibrary.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"

const FGameplayTag ASkillActor::DestroyedEventTag = PGGameplayTags::Shared_Event_ActorDestroy;


ASkillActor::ASkillActor()
{
    PrimaryActorTick.bCanEverTick = false;

    SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
    SetRootComponent(SceneRoot);

    CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
    CollisionComponent->SetupAttachment(SceneRoot);
    CollisionComponent->SetCollisionProfileName(TEXT("OverlapAllDynamic"));

    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
    ProjectileMovement->bAutoActivate = false; // 필요할 때 활성화

    ActorVFXComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("ActorVFXComponent"));
    ActorVFXComponent->SetupAttachment(SceneRoot);
    ActorVFXComponent->SetAutoActivate(false); // 필요할 때 활성화

    ActorSFXComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("ActorSFXComponent"));
    ActorSFXComponent->SetupAttachment(SceneRoot);
    ActorSFXComponent->SetAutoActivate(false); // 필요할 때 활성화
}

// 초기화
void ASkillActor::InitFromConfig(const FHeroSpawnableConfig& InConfig, const TArray<FGameplayEffectSpecHandle>& InSpecHandles)
{
    Config = InConfig;
    EffectSpecHandles = InSpecHandles;

    if(Config.Radius > 0.f)
    {
        CollisionComponent->SetSphereRadius(Config.Radius);
    }
    
    if(Config.Speed > 0.f)
    {
        ProjectileMovement->InitialSpeed = Config.Speed;
        ProjectileMovement->MaxSpeed = Config.Speed;
        ProjectileMovement->bRotationFollowsVelocity = true;
        ProjectileMovement->Activate();
    }

    if (Config.LifeSpan > 0.f)
    {
        SetLifeSpan(Config.LifeSpan);
    }
}

FGameplayTag ASkillActor::GetDestroyedEventTag() const
{
    return DestroyedEventTag;
}

void ASkillActor::BeginPlay()
{
    Super::BeginPlay();

    CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ASkillActor::OnSphereBeginOverlap);
    CollisionComponent->OnComponentEndOverlap.AddDynamic(this, &ASkillActor::OnSphereEndOverlap);

    if (Config.TickInterval > 0.f)
    {
        GetWorldTimerManager().SetTimer(
            TickEffectTimerHandle,
            this,
            &ASkillActor::HandleTickEffects,
            Config.TickInterval,
            true,
            0.f);
    }
}

// ==================================================
// 오버랩
// ==================================================
bool ASkillActor::IsValidTarget(AActor* TargetActor) const
{
    if (!TargetActor) return false;

        // TargetPolicy에 따른 타겟 유효성 검사 로직 구현 (예: 적 팀인지, 아군 팀인지, 특정 태그가 있는지 등)
    switch (Config.TargetPolicy)
    {
    case ESkillTargetPolicy::Enemy:
        return UPGFunctionLibrary::IsTargetCharacterHostile(GetOwner(), TargetActor);
    case ESkillTargetPolicy::Ally:
        return !UPGFunctionLibrary::IsTargetCharacterHostile(GetOwner(), TargetActor);
    case ESkillTargetPolicy::Self:
        return TargetActor == GetOwner();
    }

    return false;
}

void ASkillActor::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (!OtherActor) return;

    // TargetPolicy 기반 필터링
    if (!IsValidTarget(OtherActor)) return;

    if (Config.TickInterval > 0.f)
    {
        // TickInterval > 0인 경우, 타이머 기반 효과 적용을 위해 타겟 목록에 추가
        OverlappingTargets.AddUnique(OtherActor);
    }
    else
    {
        HandleInstantEffects(OtherActor);
    }
}

void ASkillActor::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (!OtherActor) return;

    // OverlappingTargets를 돌면서 OtherActor와 일치하는 항목 제거
    OverlappingTargets.RemoveAll([OtherActor](const TWeakObjectPtr<AActor>& Target) {
        return Target.Get() == OtherActor;
        });

    HitCooldownMap.Remove(OtherActor);
}

// ==================================================
// 내부 효과 처리
// ==================================================

void ASkillActor::HandleTickEffects()
{
    float CurrentTime = GetWorld()->GetTimeSeconds();

    for (TObjectPtr<AActor>& WeakTarget : OverlappingTargets)
    {
        if (!WeakTarget) continue;

        // 중복 히트 방지
        if(const float* LastHitTime = HitCooldownMap.Find(WeakTarget.Get()))
        {
            if (CurrentTime - *LastHitTime < Config.TickInterval)
            {
                continue; // 아직 쿨타임이 끝나지 않았다면 무시
            }
        }

        // 1. GE 적용
        ApplyEffectsToTarget(WeakTarget.Get());
        HitCooldownMap.Add(WeakTarget.Get(), CurrentTime);

        // 2. BP 이벤트 호출
        // BP_SkillActor 자손에서 OnTickEffectForTarget 이벤트 구현하여 이펙트 적용 시각화
        OnTickEffectForTarget(WeakTarget.Get());
    }

    // 유효하지 않은 타겟 제거
    OverlappingTargets.RemoveAll([](const TWeakObjectPtr<AActor>& Target) {
        return !Target.IsValid();
    });
}

void ASkillActor::HandleInstantEffects(AActor* TargetActor)
{
    // 1. GE 적용
    ApplyEffectsToTarget(TargetActor);

    // 2. BP 이벤트 호출
    OnInstantHit(TargetActor);

    // 투사체는 첫 충돌 후 파괴
    if (Config.Speed > 0.f)
    {
        NotifyAndDestroy();
    }
}

//=================================================
// GE 적용
//==================================================
void ASkillActor::ApplyEffectsToTarget(AActor* TargetActor)
{
    if (!TargetActor || EffectSpecHandles.IsEmpty()) return;

    UAbilitySystemComponent* TargetASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(TargetActor);
    if (!TargetASC) return;

    UAbilitySystemComponent* SourceASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(GetOwner());
    if (!SourceASC) return;

    for(const FGameplayEffectSpecHandle& SpecHandle : EffectSpecHandles)
    {
        if (!SpecHandle.IsValid()) continue;
        SourceASC->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), TargetASC);
    }

    PlayImpactVFX(TargetActor->GetActorLocation());
}

void ASkillActor::PlayImpactVFX(const FVector& Location)
{
    if (!VisualAsset) return;

    if (VisualAsset->HitVFX)
    {
        UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), VisualAsset->HitVFX, Location);
    }

    if (VisualAsset->HitSFX)
    {
        UGameplayStatics::PlaySoundAtLocation(GetWorld(), VisualAsset->HitSFX, Location);
    }
}

// ==================================================
// 파괴
// ==================================================
void ASkillActor::NotifyAndDestroy()
{
    if (bDestroyNotified) return;
    bDestroyNotified = true;

    // 루프 사운드 정지
    if (ActorSFXComponent->IsPlaying())
    {
        ActorSFXComponent->Stop();
    }

    // BP 이벤트 핀 호출
    OnSkillActorDestroyed();

    FGameplayEventData EventData;
    EventData.TargetData =
        UAbilitySystemBlueprintLibrary::AbilityTargetDataFromActor(this);

    // 파괴 이벤트 전송
    UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
        GetOwner(),
        DestroyedEventTag,
        EventData);
}

void ASkillActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    GetWorldTimerManager().ClearTimer(TickEffectTimerHandle);

    if (!bDestroyNotified)
        NotifyAndDestroy();

    Super::EndPlay(EndPlayReason);
}