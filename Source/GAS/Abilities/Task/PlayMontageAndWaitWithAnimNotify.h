#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "PlayMontageAndWaitWithAnimNotify.generated.h"

// Delegate for when the specified AnimNotify is triggered
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAnimNotifyTriggered, FName, NotifyName);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMontageEndedDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMontageBlendedOut);

UCLASS()
class GAS_API UPlayMontageAndWaitWithAnimNotify : public UAbilityTask
{
	GENERATED_BODY()

public:

	UPlayMontageAndWaitWithAnimNotify(const FObjectInitializer& ObjectInitializer);
	
    // Create the task and starts playing the montage
    UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
    static UPlayMontageAndWaitWithAnimNotify* CreatePlayMontageAndWaitWithNotifyProxy
    (
    	UGameplayAbility* OwningAbility,
    	FName TaskInstanceName,
    	UAnimMontage* MontageToPlay,
    	float Rate = 1.0f,
    	FName StartSection = NAME_None,
    	bool bStopWhenAbilityEnds = true,
    	float AnimRootMotionTranslationScale = 1.0f
    	);
	
	UPROPERTY(BlueprintAssignable)
		FOnAnimNotifyTriggered OnAnimNotifyTriggered;
	
	UPROPERTY(BlueprintAssignable)
		FOnMontageEndedDelegate OnMontageEndedDelegate;

	UPROPERTY(BlueprintAssignable)
		FOnMontageBlendedOut OnMontageBlendedOut;
	
protected:

	virtual void Activate() override;
	virtual void ExternalCancel() override;
	virtual void OnDestroy(bool bInOwnerFinished) override;

	// Called when the montage blends out
	UFUNCTION()
		void OnMontageBlendingOut(UAnimMontage* Montage, bool bInterrupted);

	// Called when the montage ends
	UFUNCTION()
		void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	// Called when an AnimNotify is triggered
	UFUNCTION()
	void OnAnimNotify(FName AnimNotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload);

	// Montage to play
	UPROPERTY()
	UAnimMontage* MontageToPlay;

	// Name of the AnimNotify to listen for
	UPROPERTY()
	FName NotifyName;

	// Playback rate
	UPROPERTY()
	float Rate;

	// Starting section of the montage
	UPROPERTY()
	FName StartSection;

	// Whether to stop the montage when the ability ends
	UPROPERTY()
	bool bStopWhenAbilityEnds;

	// Scale for root motion translation
	UPROPERTY()
	float AnimRootMotionTranslationScale;

private:
	// Handle to the montage notify delegate
	FDelegateHandle NotifyDelegateHandle;
};

