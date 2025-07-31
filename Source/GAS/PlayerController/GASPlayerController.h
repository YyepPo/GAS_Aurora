// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GAS/AbilitySystemComponent/GASAbilitySystemComponent.h"
#include "GAS/InputComponent/GASInputConfig.h"
#include "GASPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class GAS_API AGASPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	virtual void BeginPlay() override;
	
	virtual void SetupInputComponent() override;
	
private:

	UPROPERTY(EditDefaultsOnly)
		UGASInputConfig* InputConfig;

	UFUNCTION()
	void OnAbilityPressed(FGameplayTag Tag);
	UFUNCTION()
	void OnAbilityReleased(FGameplayTag Tag);

	UPROPERTY()
		TObjectPtr<UGASAbilitySystemComponent> GASAbilitySystemComponent;
	
	TObjectPtr<UGASAbilitySystemComponent> GetGASAbilitySystemComponent();
};
