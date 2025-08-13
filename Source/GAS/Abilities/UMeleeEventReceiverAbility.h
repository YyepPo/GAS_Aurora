#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "UMeleeEventReceiverAbility.generated.h"

UCLASS()
class GAS_API UUMeleeEventReceiverAbility : public UGameplayAbility
{
	GENERATED_BODY()

private:
	
	UUMeleeEventReceiverAbility();

protected:
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION()
		void OnGameplayEventReceived(FGameplayEventData Payload);

private:

	UPROPERTY()
		TObjectPtr<UAbilitySystemComponent> ASC;
		UPROPERTY()
	TObjectPtr<ACharacter> OwnerCharacter;
	
	UPROPERTY(EditAnywhere,Category = "Montage")
		TObjectPtr<UAnimMontage> ReactBackMontage;
	UPROPERTY(EditAnywhere,Category = "Montage")
		TObjectPtr<UAnimMontage> ReactLeftMontage;
	UPROPERTY(EditAnywhere,Category = "Montage")
		TObjectPtr<UAnimMontage> ReactRightMontage;
	UPROPERTY(EditAnywhere,Category = "Montage")
		TObjectPtr<UAnimMontage> ReactFrontMontage;

	UPROPERTY(EditAnywhere,Category = "Push Force",meta = (ClampMin = "1.0"))
	float PushForceMultiplier = 1.f;

	//UFUNCTION()
		//	void OnMontageFinished();

	void PlayMontageBasedOnDirection(float Angle);
	void ApplyForceToCharacterBasedOnDirection(const FVector& Direction) const;
	
};
