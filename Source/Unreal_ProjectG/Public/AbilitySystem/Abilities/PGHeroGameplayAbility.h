
#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/PGGameplayAbility.h"
#include "PGHeroGameplayAbility.generated.h"

class AHeroController;
class AHeroCharacter;
/**
 * 
 */
UCLASS()
class UNREAL_PROJECTG_API UPGHeroGameplayAbility : public UPGGameplayAbility
{
	GENERATED_BODY()
	
public:
    UPGHeroGameplayAbility();

    UFUNCTION(BlueprintPure, Category = "PG|Ability")
    AHeroCharacter* GetHeroCharacterFromActorInfo();

    UFUNCTION(BlueprintPure, Category = "PG|Ability")
    AHeroController* GetHeroControllerFromActorInfo();

    // TODO : 공격의 속성이나 다른 효과가 추가된다면 매개변수를 확장
    UFUNCTION(BlueprintPure, Category = "PG|Ability")
    FGameplayEffectSpecHandle MakeHeroDamageEffectSpecHandle(TSubclassOf<UGameplayEffect> EffectClass, float SkillMultiflier);


private:
    // TODO : 나중에 바꾸기
    TWeakObjectPtr<AHeroCharacter> CachedHeroCharacter;
    TWeakObjectPtr<AHeroController> CachedHeroController;
};
