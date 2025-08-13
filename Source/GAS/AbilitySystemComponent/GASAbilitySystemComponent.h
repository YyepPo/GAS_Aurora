// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "GASAbilitySystemComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GAS_API UGASAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	
	UGASAbilitySystemComponent();
	
	void AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& Abilities);
	void AddCharacterAbilitiesAndActivate(const TArray<TSubclassOf<UGameplayAbility>>& Abilities);
	void AddCharacterPassiveAbilities(const TArray<TSubclassOf<UGameplayAbility>>& PassiveAbilities);
		
	void RemoveCharacterAllAbilities();
	void RemoveCharacterAbility(const FGameplayAbilitySpecHandle& AbilitySpecHandle);

	void AddDefaultGameplayEffects(TSubclassOf<UGameplayEffect> DefaultAttributeEffect);

	void OnAbilityInputPressed(FGameplayTag Tag);
	void OnAbilityInputReleased( FGameplayTag Tag);

	UFUNCTION(BlueprintCallable)
		void ActivateAbilityByTag(FGameplayTag AbilityTag);
};
