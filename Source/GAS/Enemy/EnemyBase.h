// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "GAS/AbilitySystemComponent/GASAbilitySystemComponent.h"
#include "GAS/AttributeSet/GASAttributeSet.h"
#include "EnemyBase.generated.h"

UCLASS()
class GAS_API AEnemyBase : public ACharacter,public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	
	AEnemyBase();

protected:

	virtual void BeginPlay() override;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
private:

	UPROPERTY(BlueprintReadWrite,meta = (AllowPrivateAccess = true),Category = "GAS")
		TObjectPtr<UGASAbilitySystemComponent> GASAbilitySystemComponent;
	UPROPERTY(BlueprintReadWrite,meta = (AllowPrivateAccess = true),Category = "GAS")
		TObjectPtr<UGASAttributeSet> GASAttributeSet;

	UPROPERTY(EditDefaultsOnly,Category = "GAS")
		TArray<TSubclassOf<UGameplayAbility>> DefaultAbilityClasses;
	UPROPERTY(EditDefaultsOnly,Category = "GAS")
		TSubclassOf<UGameplayEffect> DefaultGameplayEffectClass;
};
