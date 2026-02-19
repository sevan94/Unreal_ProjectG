// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/PGCharacterBase.h"
#include "NativeGameplayTags.h"
#include "GameplayTagAssetInterface.h"
#include "UnitCharacter.generated.h"

class UBehaviorTree;
class UUnitData;
class UUnitCombatComponent;

/*데이터 동기화가 끝났을때 호출하는 델리게이트,
* 데이터 적용이 너무 빨라가지고 onpoesses랑 beginplay 시점에 데이터가 안들어가서 델리게이트로 해결함
*/
DECLARE_MULTICAST_DELEGATE(FOnUnitStartUpDataLoaded);

/**
 * 
 */
UCLASS()
class UNREAL_PROJECTG_API AUnitCharacter : public APGCharacterBase, public IGameplayTagAssetInterface
{
	GENERATED_BODY()
	
public:
    AUnitCharacter();

    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
    virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override;
    // 컴뱂 인터페이스 구현
    virtual UPawnCombatComponent* GetPawnCombatComponent() const override;

    FOnUnitStartUpDataLoaded OnUnitStartUpDataLoadedDelegate;

    //블랙보드 키를 넣으려고 만든 getter, 좀 무식하긴 한데 이게 제일 쉬운 방법이라 그럼
    //DetectRangeKey = 적 감지 범위 AttackRangeKey = 공격 사거리 SubBTAssetKey = 병과별 서브 트리
    //AUnitDetourCrowdAIController의 InitializeAI에서 사용함
    float GetDetectRangeKey() { return DetectRangeKey; }
    float GetAttackRangeKey() { return AttackRangeKey; }
    float GetAttackMarginKey() { return AttackMarginKey; }
    UBehaviorTree* GetSubBTAssetKey() { return SubBTAssetKey; }

    FORCEINLINE UUnitCombatComponent* GetUnitCombatComponent() const { return UnitCombatComponent; }
public:
    //공격 대상 정하는 함수
    UFUNCTION(BlueprintCallable, Category = "RVO")
    void SetAttackTarget(AActor* InTargetActor);

    // 풀에서 꺼내질 때: GAS 초기화 + AI 재가동
    virtual void ActivateUnit();

    // 풀로 돌아갈 때: AI 중지 + GAS 정리
    virtual void DeactivateUnit();

    virtual void OnDie() override;

    void SetCharacterStartupData(const TSoftObjectPtr<class UDataAsset_UnitStartupData>& InData)
    {
        CharacterStartupData = InData;
    }

protected:
    //
    virtual void PossessedBy(AController* NewController) override;

private:
    //비동기로 스타트업 데이터 로드 및 적용, 데이터 에셋에 있는 데이터를 전부 삽입하는 함수
    void InitUnitStartUpData();
public:
    //적 베이스 정하려고 만든 변수(EditAnywhere긴 한데 현재는 정하는게 의미가 없긴 함)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Movement")
    TObjectPtr<AActor> TargetActor = nullptr;
            

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UUnitCombatComponent> UnitCombatComponent;

private:
    // AI 컨트롤러 캐싱
    class AAIController* AIController = nullptr;

    class UAnimMontage* UnitAttackMontage = nullptr;

    float DetectRangeKey = 0.0f;

    float AttackRangeKey = 0.0f;

    float AttackMarginKey = 0.0f;

    bool IsActive;

    TObjectPtr<UBehaviorTree> SubBTAssetKey = nullptr;
};
