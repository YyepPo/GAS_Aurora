#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GASAbility.generated.h"

UCLASS()
class GAS_API UGASAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly)
		FGameplayTag AbilityTag;
};
