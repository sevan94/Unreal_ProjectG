
#include "Pawn/BaseStructure.h"
#include "AbilitySystem/PGAbilitySystemComponent.h"
#include "AbilitySystem/PGCharacterAttributeSet.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameplayEffectTypes.h"


ABaseStructure::ABaseStructure()
{
    PrimaryActorTick.bCanEverTick = false;

    //// 1. 캡슐 컴포넌트 설정 (루트)
    //CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComp"));
    //SetRootComponent(CapsuleComp);
    //CapsuleComp->SetCollisionProfileName(TEXT("Pawn")); 

    //// 2. 메쉬 설정
    //MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
    //MeshComp->SetupAttachment(RootComponent);
    //MeshComp->SetCollisionProfileName(TEXT("NoCollision")); 

    // 3. GAS 컴포넌트 생성
    PGAbilitySystemComponent->SetIsReplicated(true);
    
    //// 4. 공격 사거리 스피어 생성
    //AttackRangeSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AttackRangeSphere"));
    //AttackRangeSphere->SetupAttachment(RootComponent);
    //AttackRangeSphere->SetSphereRadius(800.0f); // 기본 사거리
    //AttackRangeSphere->SetCollisionProfileName(TEXT("Trigger")); // 감지 전용
}


void ABaseStructure::BeginPlay()
{
    Super::BeginPlay();

    if (PGAbilitySystemComponent)
    {
        // 초기 스탯 적용 (InitStatEffect가 있다면)
        if (InitStatEffect)
        {
            FGameplayEffectContextHandle ContextHandle = PGAbilitySystemComponent->MakeEffectContext();
            ContextHandle.AddSourceObject(this);
            FGameplayEffectSpecHandle SpecHandle = PGAbilitySystemComponent->MakeOutgoingSpec(InitStatEffect, 1.0f, ContextHandle);

            if (SpecHandle.IsValid())
            {
                PGAbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
            }
        }

    }

    PGAbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
        CharacterAttributeSet->GetHealthAttribute()).AddUObject(this, &ABaseStructure::CurrentHealthChange);
    PGAbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
        CharacterAttributeSet->GetMaxHealthAttribute()).AddUObject(this, &ABaseStructure::MaxHealthChange);

    //// 오버랩 이벤트
    //AttackRangeSphere->OnComponentBeginOverlap.AddDynamic(this, &ABaseStructure::OnAttackRangeBeginOverlap);
    //AttackRangeSphere->OnComponentEndOverlap.AddDynamic(this, &ABaseStructure::OnAttackRangeEndOverlap);

    // 공격 타이머 시작 (AttackRate 초마다 ProcessAttack 실행)
    GetWorld()->GetTimerManager().SetTimer(AttackTimerHandle, this, &ABaseStructure::ProcessAttack, AttackRate, true);
}

// 사거리에 누군가 들어왔을 때
void ABaseStructure::OnAttackRangeBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && OtherActor != this)
    {
        UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor);

        
        // TargetTags에 등록된 태그 중 '하나라도' 가지고 있다면 타겟으로 인정!
        if (TargetASC && TargetASC->HasAnyMatchingGameplayTags(TargetTags))
        {
            TargetList.AddUnique(OtherActor);
        }
    }
}

// 사거리에서 벗어났을 때
void ABaseStructure::OnAttackRangeEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (OtherActor)
    {
        TargetList.Remove(OtherActor); // 타겟 목록에서 제외
    }
}

void ABaseStructure::ProcessAttack()
{
    // 죽었거나 파괴된 액터는 리스트에서 청소
    TargetList.RemoveAll([](AActor* Actor) { return !IsValid(Actor); });

    // 때릴 타겟이 있다면
    if (TargetList.Num() > 0)
    {
        AActor* CurrentTarget = TargetList[0]; // 사거리에 가장 먼저 들어온 타겟을 우선 공격

        if (AttackDamageEffect && PGAbilitySystemComponent)
        {
            UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(CurrentTarget);
            if (TargetASC)
            {
                // 데미지 이펙트(GE) 적용 -> 투사체 스폰 로직 추가 가능
                FGameplayEffectContextHandle Context = PGAbilitySystemComponent->MakeEffectContext();
                Context.AddSourceObject(this);
                FGameplayEffectSpecHandle Spec = PGAbilitySystemComponent->MakeOutgoingSpec(AttackDamageEffect, 1.0f, Context);

                PGAbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*Spec.Data.Get(), TargetASC);

                // 시각 효과 처리를 위해 BP 이벤트 호출
                BP_OnBaseAttack(CurrentTarget);
            }
        }
    }
}

UAbilitySystemComponent* ABaseStructure::GetAbilitySystemComponent() const
{
    return PGAbilitySystemComponent;
}

void ABaseStructure::DestroyBase()
{
    // 이미 파괴된 상태라면 무시
    if (!this->IsValidLowLevel() || IsActorBeingDestroyed()) return;

    UE_LOG(LogTemp, Warning, TEXT("Base Destroyed! Team: %d"), (int32)TeamID);

    // 1. 게임 모드에 알림 (승패 판정)
    if (OnBaseDestroyed.IsBound())
    {
        OnBaseDestroyed.Broadcast(TeamID);
    }

    // 2. 기지 파괴
    Destroy();
}

void ABaseStructure::CurrentHealthChange(const FOnAttributeChangeData& Data) const
{
    OnBaseHpChanged.Broadcast(TeamTag, Data.NewValue);
}
void ABaseStructure::MaxHealthChange(const FOnAttributeChangeData& Data) const
{
    OnBaseMaxHpChanged.Broadcast(TeamTag, Data.NewValue);
}