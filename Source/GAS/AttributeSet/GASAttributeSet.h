 // Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "GASAttributeSet.generated.h"

// Macro to define getter, setter, and a static function to get the attribute itself
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class GAS_API UGASAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

protected:
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	
	// This function is used to modify attributes with the help of ATTRIBUTE_ACCESSORS macro
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

public:

	// Mana Attribute
	UPROPERTY(BlueprintReadOnly,ReplicatedUsing = OnRep_Mana, BlueprintReadOnly, Category = "Attributes")
		FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS(UGASAttributeSet,Mana);
	UFUNCTION()
		void OnRep_Mana(const FGameplayAttributeData& OldMana);
	
	// Max Mana Attribute
	UPROPERTY(BlueprintReadOnly,ReplicatedUsing = OnRep_MaxMana, BlueprintReadOnly, Category = "Attributes")
		FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS(UGASAttributeSet,MaxMana)
	UFUNCTION()
		void OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana);

};
