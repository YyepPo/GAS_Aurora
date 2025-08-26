// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "GAS/Abilities/GASAbility.h"
#include "GAS/AbilitySystemComponent/GASAbilitySystemComponent.h"
#include "GAS/Interfaces/GASCharacterInterface.h"
#include "GASCharacterBase.generated.h"

UCLASS()
class GAS_API AGASCharacterBase : public ACharacter,public IAbilitySystemInterface,public IGASCharacterInterface
{
	GENERATED_BODY()

public:
	
	AGASCharacterBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void InitAbilityInfo();
	virtual void BindToAttributeCallbacks();

	//~ GAS Properties
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta = (AllowPrivateAccess = true),Category = "GAS")
		TObjectPtr<UGASAbilitySystemComponent> GASAbilitySystemComponent;
	//~ End GAS Properties

	//~ Ability System Interface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	//~ End Ability System Interface
	
	//~ IGASCharacterInterface
	virtual void Death_Implementation() override;
	virtual void AddExperience_Implementation(const FScalableFloat& Experience) override;
	virtual void AddHitActor_Implementation(AActor* Actor) override;
	//~ End IGASCharacterInterface

	//~ Death
	UFUNCTION(Server,Reliable)
		void Server_SetIsDead();
		
	UPROPERTY(ReplicatedUsing=OnRep_IsDead)
		bool bIsDead = false;
	UFUNCTION()
		void OnRep_IsDead();
	
	// It gets called inside a replicated function
    	virtual void OnDeath() {};
	
	UPROPERTY(EditDefaultsOnly,Category = "GAS|Ability")
		TSubclassOf<UGASAbility> DeathAbilityClass;
	//~ End Death

	UPROPERTY(EditDefaultsOnly,Category = "GAS|Tag")
		FGameplayTag CharacterTag;
	
private:
	
	//~ Death
	
	//~ End Death
};
