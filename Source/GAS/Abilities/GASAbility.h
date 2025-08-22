#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GASAbility.generated.h"

UCLASS()
class GAS_API UGASAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
public:

	UGASAbility();

	UPROPERTY(EditDefaultsOnly)
		FGameplayTag InputAbilityTag;
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
		float GameplayCooldown = 0.1;
};
