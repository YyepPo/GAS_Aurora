// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/AttributeSet/GASHealthAttributeSet.h"
#include "GAS/Character/GASCharacterBase.h"
#include "EnemyBase.generated.h"

UCLASS()
class GAS_API AEnemyBase : public AGASCharacterBase
{
	GENERATED_BODY()

public:
	
	AEnemyBase();

protected:

	virtual void BeginPlay() override;

	
	virtual void InitAbilityInfo() override;
	virtual void BindToAttributeCallbacks() override;

	//~ Death
	virtual void Death_Implementation() override;
	virtual void OnDeath() override;
	//~ End Death

	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta = (AllowPrivateAccess = "true"))
		TObjectPtr<class UWidgetComponent> OverheadWidgetComponent;
	
public:

    //~ Attribute Change Delegates
	UFUNCTION(BlueprintImplementableEvent)
		void OnHealthChanged(float CurrentValue,float MaxHealth);
	UFUNCTION(BlueprintImplementableEvent)
		void OnArmorChanged(float CurrentValue,float MaxArmor);
	//~ End Attribute Change Delegates
	
private:

	//~ GAS Properties
	UPROPERTY(BlueprintReadWrite,meta = (AllowPrivateAccess = true),Category = "GAS")
		TObjectPtr<UGASHealthAttributeSet> HealthAttributeSet;
	//~ End GAS Properties
	
	void InitializeCharacterInfo();
};
