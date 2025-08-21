// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent/GASAbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "AttributeSet/GASAttributeSet.h"
#include "AttributeSet/GASHealthAttributeSet.h"
#include "GameFramework/PlayerState.h"
#include "GASPlayerState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthValueChanged,float,NewValue,float,MaxHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnManaValueChanged,float,NewValue,float,MaxMana);

UCLASS()
class GAS_API AGASPlayerState : public APlayerState,public IAbilitySystemInterface
{
	GENERATED_BODY()

	AGASPlayerState();
	
public:
	
	virtual void BeginPlay() override;
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	// Get Ability System Comp
	UFUNCTION(BlueprintPure)
		FORCEINLINE UGASAbilitySystemComponent* GetAbilitySystemComp() const {return AbilitySystemComponent;}

	// Get AttributeSet
	UFUNCTION(BlueprintPure)
		FORCEINLINE UGASAttributeSet* GetAttributeSet() const {return AttributeSet;}
	UFUNCTION(BlueprintPure)
		FORCEINLINE UGASHealthAttributeSet* GetHealthAttributeSet() const {return HealthAttributeSet;}
	
	UPROPERTY(BlueprintAssignable)
		FOnHealthValueChanged OnHealthValueChanged;
	UPROPERTY(BlueprintAssignable)
		FOnManaValueChanged OnManaValueChanged;
	
private:

	UPROPERTY(BlueprintReadOnly,meta = (AllowPrivateAccess = true))
		TObjectPtr<UGASAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(BlueprintReadOnly,meta = (AllowPrivateAccess = true))
		TObjectPtr<UGASAttributeSet> AttributeSet;
	UPROPERTY(BlueprintReadOnly,meta = (AllowPrivateAccess = true))
		TObjectPtr<UGASHealthAttributeSet> HealthAttributeSet;
};
