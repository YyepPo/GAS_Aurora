// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "GASHealthAttributeSet.generated.h"


// Macro to define getter, setter, and a static function to get the attribute itself
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class GAS_API UGASHealthAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
protected:
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	
	// This function is used to modify attributes with the help of ATTRIBUTE_ACCESSORS macro
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

public:
	
	// Health Attribute
	UPROPERTY(BlueprintReadOnly,ReplicatedUsing = OnRep_Health,Category = "Attributes")
		FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UGASHealthAttributeSet, Health);
	UFUNCTION()
		void OnRep_Health(const FGameplayAttributeData& OldHealth);
	
	// Max Health Attribute
	UPROPERTY(BlueprintReadOnly,ReplicatedUsing = OnRep_Health,Category = "Attributes")
		FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UGASHealthAttributeSet, MaxHealth);
	UFUNCTION()
		void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth);

	// Armor Attribute
	UPROPERTY(BlueprintReadOnly,ReplicatedUsing = OnRep_Armor, BlueprintReadOnly, Category = "Attributes")
		FGameplayAttributeData Armor;
	ATTRIBUTE_ACCESSORS(UGASHealthAttributeSet,Armor)
	UFUNCTION()
		void OnRep_Armor(const FGameplayAttributeData& OldArmor);

	UPROPERTY(BlueprintReadOnly,ReplicatedUsing = OnRep_MaxArmor, BlueprintReadOnly, Category = "Attributes")
		FGameplayAttributeData MaxArmor;
	ATTRIBUTE_ACCESSORS(UGASHealthAttributeSet,MaxArmor)
	UFUNCTION()
		void OnRep_MaxArmor(const FGameplayAttributeData& OldMaxArmor);
	
	// Damage is a meta attribute used by the DamageExecution to calculate final damage, which then turns into -Health
    // Temporary value that only exists on the Server. Not replicated.
    UPROPERTY(BlueprintReadOnly, Category = "Damage")
    	FGameplayAttributeData Damage;
    ATTRIBUTE_ACCESSORS(UGASHealthAttributeSet, Damage)
};
