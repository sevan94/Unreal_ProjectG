#include "Actors/PGMageMagicBase.h"
#include "Components/BoxComponent.h"
#include "NiagaraComponent.h"
#include "PGFunctionLibrary.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "PGGameplayTags.h"
#include "Character/Unit/SubSystem/PGObjectPoolSubsystem.h"

APGMageMagicBase::APGMageMagicBase()
{
    PrimaryActorTick.bCanEverTick = false;

    RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootSceneComponent"));
    SetRootComponent(RootSceneComponent);


    MagicCollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("MagicCollisionComponent"));
    MagicCollisionComponent->SetupAttachment(RootSceneComponent); 

    MagicCollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    MagicCollisionComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
    MagicCollisionComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
    MagicCollisionComponent->SetGenerateOverlapEvents(true);

    MagicNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("MagicNiagaraComponent"));
    MagicNiagaraComponent->SetupAttachment(RootSceneComponent);
    MagicNiagaraComponent->SetUsingAbsoluteRotation(false);

    MagicCollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &APGMageMagicBase::OnMagicBeginOverlap);

}

void APGMageMagicBase::BeginPlay()
{
    Super::BeginPlay();
    MagicCollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    MagicNiagaraComponent->OnSystemFinished.AddDynamic(this, &APGMageMagicBase::OnNiagaraFinished);
}

void APGMageMagicBase::OnMagicBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (!UPGFunctionLibrary::IsTargetCharacterHostile(GetInstigator(), OtherActor))
    {
        return;
    }

    APawn* TargetPawn = Cast<APawn>(OtherActor);
    if (TargetPawn)
    {
        for (const FGameplayEffectSpecHandle& SpecHandle : MagicDamageEffectSpecHandles)
        {
            if (SpecHandle.IsValid())
            {
                UPGFunctionLibrary::ApplyGameplayEffectSpecHandleToTargetActor(GetInstigator(), TargetPawn, SpecHandle);
            }
        }
        UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(TargetPawn, PGGameplayTags::Shared_Event_HitReact, FGameplayEventData());
    }
}

// --- [풀링: 창고에서 꺼낼 때 실행되는 초기화 로직] ---
void APGMageMagicBase::OnActivatedFromPool_Implementation()
{
    // 1. 콜리전 다시 켜기
    if (MagicCollisionComponent)
    {
        MagicCollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    }

    // 2. 나이아가라 이펙트 다시 재생하기
    if (MagicNiagaraComponent)
    {
        MagicNiagaraComponent->Activate(true); // 이펙트 초기화 및 재생
    }

    // 3. 액터 보이기
    SetActorHiddenInGame(false);
}

// --- [풀링: 창고로 들어갈 때 실행되는 정리 로직] ---
void APGMageMagicBase::OnReturnedToPool_Implementation()
{
    // 1. 콜리전 끄기 (창고 안에서 데미지 판정 방지)
    if (MagicCollisionComponent)
    {
        MagicCollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    }

    // 2. 이펙트 정지
    if (MagicNiagaraComponent)
    {
        MagicNiagaraComponent->Deactivate();
    }

    // 3. 액터 숨기기
    SetActorHiddenInGame(true);
}

// 풀 반납 도우미 함수
void APGMageMagicBase::DeactivateAndReturnToPool()
{
    OnReturnedToPool();

    if (UPGObjectPoolSubsystem* PoolSubsystem = GetWorld()->GetSubsystem<UPGObjectPoolSubsystem>())
    {
        PoolSubsystem->ReturnActorToPool(this);
    }
}

void APGMageMagicBase::OnNiagaraFinished(UNiagaraComponent* PSystem)
{
    DeactivateAndReturnToPool();
}

void APGMageMagicBase::SetMagicDamageEffectSpecHandle(const TArray<FGameplayEffectSpecHandle>& InSpecHandles)
{
    MagicDamageEffectSpecHandles = InSpecHandles;
}