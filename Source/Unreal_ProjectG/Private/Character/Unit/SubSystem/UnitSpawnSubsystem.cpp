// UnitSpawnSubsystem.cpp

#include "Character/Unit/SubSystem/UnitSpawnSubsystem.h"
#include "Character/Unit/UnitCharacter.h"
#include "Engine/World.h"
#include "DataAssets/StartUp/DataAsset_UnitStartupData.h"

void UUnitSpawnSubsystem::PrewarmPool(TSubclassOf<AUnitCharacter> UnitClass, int32 Count)
{
    if (!GetWorld() || !UnitClass) return;

    FUnitPoolList& PoolList = PoolStorage.FindOrAdd(UnitClass);
    int32 CurrentCount = PoolList.Units.Num();
    int32 NeededCount = Count - CurrentCount;

    if (NeededCount <= 0) return;

    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    for (int32 i = 0; i < NeededCount; i++)
    {
        AUnitCharacter* NewUnit = GetWorld()->SpawnActor<AUnitCharacter>(UnitClass, FVector(0, 0, -10000.0f), FRotator::ZeroRotator, SpawnParams);
        if (NewUnit)
        {
            NewUnit->DeactivateUnit();
            PoolList.Units.Add(NewUnit);
        }
    }
}

AUnitCharacter* UUnitSpawnSubsystem::GetUnitInstance(TSubclassOf<AUnitCharacter> UnitClass)
{
    if (!UnitClass || !GetWorld()) return nullptr;

    FUnitPoolList& PoolList = PoolStorage.FindOrAdd(UnitClass);
    AUnitCharacter* UnitToReturn = nullptr;

    while (PoolList.Units.Num() > 0)
    {
        UnitToReturn = PoolList.Units.Pop();
        if (IsValid(UnitToReturn))
        {
            break;
        }
        UnitToReturn = nullptr;
    }

    // 2. 없으면 새로 생성
    if (!UnitToReturn)
    {
        FActorSpawnParameters SpawnParams;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
        UnitToReturn = GetWorld()->SpawnActor<AUnitCharacter>(UnitClass, FVector(0, 0, -10000.0f), FRotator::ZeroRotator, SpawnParams);
    }

    return UnitToReturn;
}

void UUnitSpawnSubsystem::SpawnUnit(TSubclassOf<AUnitCharacter> UnitClass, 
    TSoftObjectPtr<UDataAsset_UnitStartupData> UnitData,
    FVector Location, FRotator Rotation,
    AActor* InTargetActor)
{
    if (!UnitClass) return;
    AUnitCharacter* SpawnedUnit = GetUnitInstance(UnitClass);
    if (SpawnedUnit)
    {
        SpawnedUnit->SetActorLocationAndRotation(Location, Rotation);
        SpawnedUnit->SetCharacterStartupData(UnitData);
        if (InTargetActor)
        {
            SpawnedUnit->SetAttackTarget(InTargetActor);
        }
        ActiveUnits.Add(SpawnedUnit);
        SpawnedUnit->ActivateUnit();
    }

    if (!UnitClass || !GetWorld()) return;

    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    //AUnitCharacter* NewUnit = GetWorld()->SpawnActor<AUnitCharacter>(UnitClass, Location, Rotation, SpawnParams);
    //if (NewUnit)
    //{
    //    // 데이터 설정
    //    NewUnit->SetCharacterStartupData(UnitData);
    //    // 목록 관리가 필요하다면 추가 (선택사항)
    //    ActiveUnits.Add(NewUnit);
    //}
}

void UUnitSpawnSubsystem::ReturnToPool(AUnitCharacter* Unit)
{
    if (!IsValid(Unit)) return;

    Unit->DeactivateUnit();

    // 유닛의 클래스를 키값으로 사용하여 풀에 저장
    TSubclassOf<AUnitCharacter> UnitClass = Unit->GetClass();
    FUnitPoolList& PoolList = PoolStorage.FindOrAdd(UnitClass);

    PoolList.Units.Add(Unit);

    // 활성 목록에서 제거
    ActiveUnits.RemoveAll([Unit](const TWeakObjectPtr<AUnitCharacter>& Ptr)
        {
            return !Ptr.IsValid() || Ptr.Get() == Unit;
        });
}

void UUnitSpawnSubsystem::GetAllActiveUnits(TArray<AUnitCharacter*>& OutUnits)
{
    OutUnits.Empty();
    for (int32 i = ActiveUnits.Num() - 1; i >= 0; --i)
    {
        if (ActiveUnits[i].IsValid())
        {
            OutUnits.Add(ActiveUnits[i].Get());
        }
        else
        {
            ActiveUnits.RemoveAt(i);
        }
    }
}

void UUnitSpawnSubsystem::DespawnAllActiveUnits()
{
    TArray<TWeakObjectPtr<AUnitCharacter>> TempUnits = ActiveUnits;

    for (const auto& WeakUnit : TempUnits)
    {
        if (AUnitCharacter* Unit = WeakUnit.Get())
        {
            ReturnToPool(Unit);
        }
    }
    ActiveUnits.Empty();
}

void UUnitSpawnSubsystem::OnUnitDied(AUnitCharacter* Victim)
{
    if (IsValid(Victim))
    {
        // Destroy 대신 풀 반환 호출
        ReturnToPool(Victim);
        ActiveUnits.Remove(Victim);

        // [변경점] ReturnToPool 대신 그냥 Destroy
        //Victim->Destroy();

    }
}