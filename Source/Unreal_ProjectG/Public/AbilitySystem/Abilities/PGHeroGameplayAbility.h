
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

private:
    // TODO : 나중에 바꾸기
    TWeakObjectPtr<AHeroCharacter> CachedHeroCharacter;
    TWeakObjectPtr<AHeroController> CachedHeroController;
};
