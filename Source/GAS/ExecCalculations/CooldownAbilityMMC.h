#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "CooldownAbilityMMC.generated.h"

UCLASS()
class GAS_API UCooldownAbilityMMC : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()

public:

	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;
	
};
