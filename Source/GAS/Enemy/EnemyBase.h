// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GAS/AbilitySystemComponent/GASAbilitySystemComponent.h"
#include "GAS/AttributeSet/GASAttributeSet.h"
#include "GAS/Character/GASCharacterBase.h"
#include "EnemyBase.generated.h"

UCLASS()
class GAS_API AEnemyBase : public AGASCharacterBase,public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	
	AEnemyBase();

protected:

	virtual void BeginPlay() override;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual void InitAbilityInfo() override;
	virtual void BindToAttributeCallbacks() override;
	
public:

	UFUNCTION(BlueprintImplementableEvent)
		void OnHealthChanged(float CurrentValue,float MaxHealth);
	UFUNCTION(BlueprintImplementableEvent)
		void OnArmorChanged(float CurrentValue,float MaxArmor);
	
private:

	UPROPERTY(BlueprintReadWrite,meta = (AllowPrivateAccess = true),Category = "GAS")
		TObjectPtr<UGASAbilitySystemComponent> GASAbilitySystemComponent;
	UPROPERTY(BlueprintReadWrite,meta = (AllowPrivateAccess = true),Category = "GAS")
		TObjectPtr<UGASAttributeSet> GASAttributeSet;

	void InitializeCharacterInfo();
	UPROPERTY(EditDefaultsOnly,meta = (AllowPrivateAccess = true),Category = "GAS|Tag")
	FGameplayTag CharacterTag;
};
