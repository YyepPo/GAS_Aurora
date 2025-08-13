// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GAS/AttributeSet/GASAttributeSet.h"
#include "GASCharacterBase.generated.h"

UCLASS()
class GAS_API AGASCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	
	AGASCharacterBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	
	virtual void BeginPlay() override;
	virtual void InitAbilityInfo();
	virtual void BindToAttributeCallbacks();

	//~ GAS Properties
	UPROPERTY(EditDefaultsOnly,Category = "GAS")
		TArray<TSubclassOf<UGameplayAbility>> DefaultAbilityClasses;
	UPROPERTY(EditDefaultsOnly,Category = "GAS")
		TSubclassOf<UGameplayEffect> DefaultGameplayEffectClass;
	//~ End GAS Properties
};
