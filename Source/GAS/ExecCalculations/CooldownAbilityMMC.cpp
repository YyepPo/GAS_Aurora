// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/ExecCalculations/CooldownAbilityMMC.h"

#include "GAS/Abilities/GASAbility.h"

float UCooldownAbilityMMC::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const UGASAbility* Ability = Cast<UGASAbility>(Spec.GetContext().GetAbility());
	if (IsValid(Ability))
	{
		float Cooldown = Ability->GameplayCooldown;
		if (Cooldown <= 0.0f)
		{
			UE_LOG(LogTemp, Warning, TEXT("CooldownAbilityMMC: GameplayCooldown is %f, returning 0.1 to avoid activation issues"), Cooldown);
			return 0.1f; // Prevent zero or negative cooldowns
		}
		UE_LOG(LogTemp, Log, TEXT("CooldownAbilityMMC: Applying cooldown of %f seconds for ability %s"), Cooldown, *GetNameSafe(Ability));
		return Cooldown;
	}

	UE_LOG(LogTemp, Warning, TEXT("CooldownAbilityMMC: Ability is invalid, returning default cooldown of 0.1 seconds"));
	return 0.1f; // Fallback to avoid ability activation issues
}