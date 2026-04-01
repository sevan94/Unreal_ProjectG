// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Unit/SubSystem/PGObjectPoolSubsystem.h"

AActor* UPGObjectPoolSubsystem::GetActorFromPool(TSubclassOf<AActor> ActorClass, const FTransform& SpawnTransform)
{
    if (!ActorClass) return nullptr;

    TArray<AActor*>& Pool = ActorPool.FindOrAdd(ActorClass).Actors;

    // 1. 풀에 남은 액터가 있다면 꺼내서 재사용
    while (Pool.Num() > 0)
    {
        AActor* PooledActor = Pool.Pop();

        // 널 포인터(중간에 파괴된 액터) 방어 로직
        if (IsValid(PooledActor))
        {
            // 위치와 회전값만 원하는 곳으로 이동
            //숨김 해제, 콜리전 켜기 등은 액터 내부의 OnActivatedFromPool에서 알아서 진행
            PooledActor->SetActorTransform(SpawnTransform);

            return PooledActor;
        }
    }

    // 2. 풀 비어있다면 새로 스폰 (최초 실행 시)
    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    AActor* NewActor = GetWorld()->SpawnActor<AActor>(ActorClass, SpawnTransform, SpawnParams);

    return NewActor;
}

void UPGObjectPoolSubsystem::ReturnActorToPool(AActor* ActorToReturn)
{
    if (!IsValid(ActorToReturn)) return;


    TArray<AActor*>& Pool = ActorPool.FindOrAdd(ActorToReturn->GetClass()).Actors;
    Pool.AddUnique(ActorToReturn);
}

void UPGObjectPoolSubsystem::PrewarmPool(TSubclassOf<AActor> ActorClass, int32 PoolSize)
{
    if (!ActorClass) return;

    TArray<AActor*>& Pool = ActorPool.FindOrAdd(ActorClass).Actors;

    for (int32 i = 0; i < PoolSize; ++i)
    {
        FActorSpawnParameters SpawnParams;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

        // 플레이어 눈에 안 띄는 맵 저 밑바닥에 스폰
        FTransform SpawnTransform(FRotator::ZeroRotator, FVector(0.f, 0.f, -10000.f));
        AActor* NewActor = GetWorld()->SpawnActor<AActor>(ActorClass, SpawnTransform, SpawnParams);

        if (NewActor)
        {
            // 스폰 직후 풀 반납 로직 실행 (숨기고, 콜리전 끄고, 틱 끄기)
            // (Projectile이나 Magic 클래스로 캐스팅해서 OnReturnedToPool을 강제로 한 번 호출해주거나, 아래처럼 수동 처리)
            NewActor->SetActorHiddenInGame(true);
            NewActor->SetActorEnableCollision(false);

            Pool.AddUnique(NewActor);
        }
    }
}