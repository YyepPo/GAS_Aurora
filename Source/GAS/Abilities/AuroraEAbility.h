// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/Abilities/GASAbility.h"
#include "AuroraEAbility.generated.h"

struct FGameplayCueTag;

UCLASS()
class GAS_API UAuroraEAbility : public UGASAbility
{
	GENERATED_BODY()
	
public:
	
	UAuroraEAbility();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:
	
	UFUNCTION()
		void OnMontageCompleted();
	UFUNCTION()
		void OnMontageInterrupted();
	UFUNCTION()
		void OnMontageAnimNotifyTriggered(FName AnimNotifyText);
	
	UFUNCTION(BlueprintCallable,BlueprintImplementableEvent)
		void SpawnIceSparksVFXReplicated(const TArray<FVector_NetQuantize>& Locations,const TArray<FVector_NetQuantize>& LookAtRotations,const TArray<float>& EmitterScales);
	// Whether to disable or enable movement
	UFUNCTION(Blueprintable,BlueprintImplementableEvent)
		void EnableMovement(bool bEnabled);
private:
	
	bool IsIceSpikeDataAssetValid() const;

	void FreezeEnemies(const FVector_NetQuantize& Location);

	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta = (AllowPrivateAccess = true))
		class UIceSpikeDataAsset* IceSpikeDataAsset;
};
