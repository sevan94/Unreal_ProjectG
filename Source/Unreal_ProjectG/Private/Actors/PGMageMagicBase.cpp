#include "Actors/PGMageMagicBase.h"
#include "Components/BoxComponent.h"
#include "NiagaraComponent.h"
#include "PGFunctionLibrary.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "PGGameplayTags.h"

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

void APGMageMagicBase::OnNiagaraFinished(UNiagaraComponent* PSystem)
{
    Destroy();
}

void APGMageMagicBase::SetMagicDamageEffectSpecHandle(const TArray<FGameplayEffectSpecHandle>& InSpecHandles)
{
    MagicDamageEffectSpecHandles = InSpecHandles;
}