#include "Actors/PGProjectileBase.h"
#include "Components/BoxComponent.h"
#include "NiagaraComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "PGFunctionLibrary.h"
#include "PGGameplayTags.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"

APGProjectileBase::APGProjectileBase()
{
	PrimaryActorTick.bCanEverTick = false;

    ProjectileCollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("ProjectileCollisionComponent"));
    SetRootComponent(ProjectileCollisionComponent);
    ProjectileCollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    ProjectileCollisionComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
    ProjectileCollisionComponent->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
    ProjectileCollisionComponent->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
    ProjectileCollisionComponent->OnComponentHit.AddUniqueDynamic(this, &APGProjectileBase::OnProjectileHit);
    ProjectileCollisionComponent->OnComponentBeginOverlap.AddUniqueDynamic(this, &APGProjectileBase::OnProjectileBeginOverlap);

    ProjectileNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("ProjectileNiagaraComponent"));
    ProjectileNiagaraComponent->SetupAttachment(RootComponent);

    ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
    ProjectileMovementComponent->InitialSpeed = 2000.f;
    ProjectileMovementComponent->MaxSpeed = 2000.f;
    ProjectileMovementComponent->Velocity = FVector(1.f, 0.f, 0.f);
    ProjectileMovementComponent->ProjectileGravityScale = 0.f;

    InitialLifeSpan = ProjectileSpan;
}

void APGProjectileBase::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    FVector BoxExtent = ProjectileCollisionComponent->GetScaledBoxExtent();
    DrawDebugBox(GetWorld(), GetActorLocation(), BoxExtent, FColor::Red, false, -1.f, 0, 1.f);
}

void APGProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	
    //checkf(GetInstigator(), TEXT("Projectile %s has no valid Instigator assigned!"), *GetName());

    if (AActor* OwnerActor = GetInstigator())
    {
        ProjectileCollisionComponent->IgnoreActorWhenMoving(GetInstigator(), true);
    }

    // 데미지 적용 정책에 따른 콜리전 응답 설정
    if (ProjectileDamagePolicy == EProjectileDamagePolicy::OnHit)
    {
        ProjectileCollisionComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
    }
}

void APGProjectileBase::OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    UE_LOG(LogTemp, Log, TEXT("Hit With %s"), *OtherActor->GetName());

    // 히트한 액터가 플레이어면 무시
    if (OtherActor == GetInstigator())
    {
        return;
    }

    APawn* HitPawn = Cast<APawn>(OtherActor);
    // 맞은 액터가 Pawn이면 데미지 적용
    if(HitPawn)
    {
        FGameplayEventData Data;
        Data.Instigator = this;
        Data.Target = HitPawn;

        HandleApplyProjectileDamage(HitPawn, Data);
        if (HitImpactVFX.IsValid())
        {
            UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), HitImpactVFX.Get(), Hit.ImpactPoint, Hit.ImpactNormal.Rotation());
        }
    }



    Destroy();
}

void APGProjectileBase::OnProjectileBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    // 오버랩한 액터가 같은 팀이면 무시
    if(!UPGFunctionLibrary::IsTargetCharacterHostile(GetInstigator(), OtherActor))
    {
        return;
    }
     UE_LOG(LogTemp, Log, TEXT("Overlap With %s"), *OtherActor->GetName());

    APawn* OverlappedPawn = Cast<APawn>(OtherActor);
    if (OverlappedPawn)
    {
        FGameplayEventData Data;
        Data.Instigator = this;
        Data.Target = OverlappedPawn;

        HandleApplyProjectileDamage(OverlappedPawn, Data);
        if (HitImpactVFX.IsValid())
        {
            UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), HitImpactVFX.Get(), SweepResult.ImpactPoint, SweepResult.ImpactNormal.Rotation());
        }
        // 히트된 액터에게 히트 반응 이벤트 전송
        UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(OverlappedPawn, PGGameplayTags::Shared_Event_HitReact, FGameplayEventData());
    }


    // 이펙트와 사운드가 유효하다면 재생
    if(ProjectileImpactVFX.IsValid())
    {
        UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ProjectileImpactVFX.Get(), GetActorLocation(), GetActorRotation());
    }
    if(ProjectileImpactSFX.IsValid())
    {
        UGameplayStatics::PlaySoundAtLocation(GetWorld(), ProjectileImpactSFX.Get(), GetActorLocation());
    }
    Destroy();
}

void APGProjectileBase::SetProjectileDamageEffectSpecHandle(const FGameplayEffectSpecHandle& InEffectSpecHandle)
{
    ProjectileDamageEffectSpecHandle = InEffectSpecHandle;
}

void APGProjectileBase::HandleApplyProjectileDamage(APawn* InHitPawn, const FGameplayEventData& InPayload)
{
    checkf(ProjectileDamageEffectSpecHandle.IsValid(), TEXT("Forgot to assign a valid ProjectileDamageEffectSpecHandle to the projectile %s"), *GetName());

    UPGFunctionLibrary::ApplyGameplayEffectSpecHandleToTargetActor(GetInstigator(), InHitPawn, ProjectileDamageEffectSpecHandle);
}
