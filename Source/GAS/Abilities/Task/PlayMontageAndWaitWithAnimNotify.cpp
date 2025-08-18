// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayMontageAndWaitWithAnimNotify.h"

#include "AbilitySystemComponent.h"
#include "GameFramework/Character.h"

UPlayMontageAndWaitWithAnimNotify::UPlayMontageAndWaitWithAnimNotify(const FObjectInitializer& ObjectInitializer):
Super(ObjectInitializer)
{
	bStopWhenAbilityEnds = true;
}

UPlayMontageAndWaitWithAnimNotify* UPlayMontageAndWaitWithAnimNotify::CreatePlayMontageAndWaitWithNotifyProxy(
	UGameplayAbility* OwningAbility,
	FName TaskInstanceName,
	UAnimMontage* MontageToPlay,
	float Rate,
	FName StartSection,
	bool bStopWhenAbilityEnds,
	float AnimRootMotionTranslationScale)
{
	UAbilitySystemComponent* ASC = OwningAbility->GetAbilitySystemComponentFromActorInfo();
	if (!ASC)
	{
		return nullptr;
	}

	UPlayMontageAndWaitWithAnimNotify* Task = NewAbilityTask<UPlayMontageAndWaitWithAnimNotify>(OwningAbility, TaskInstanceName);
	Task->MontageToPlay = MontageToPlay;
	Task->Rate = Rate;
	Task->StartSection = StartSection;
	Task->bStopWhenAbilityEnds = bStopWhenAbilityEnds;
	Task->AnimRootMotionTranslationScale = AnimRootMotionTranslationScale;

	return Task;
}

void UPlayMontageAndWaitWithAnimNotify::Activate()
{
	Super::Activate();

	if(IsValid(Ability) == false || AbilitySystemComponent.IsValid() == false || MontageToPlay == nullptr)
	{
		return;
	}

	ACharacter* Character = Cast<ACharacter>(GetAvatarActor());
	if (!Character)
	{
		EndTask();
		return;
	}

	UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();
	if (!AnimInstance)
	{
		EndTask();
		return;
	}

	// Play the montage
	const float Duration = AbilitySystemComponent->PlayMontage(Ability,FGameplayAbilityActivationInfo(),MontageToPlay,Rate,StartSection,0);
	if (Duration <= 0.0f)
	{
		EndTask();
		return;
	}

	// Bind to montage blending out (completion or interruption)
	AnimInstance->OnMontageBlendingOut.AddDynamic(this, &UPlayMontageAndWaitWithAnimNotify::OnMontageBlendingOut);

	// Bind to montage ended
	AnimInstance->OnMontageEnded.AddDynamic(this,&UPlayMontageAndWaitWithAnimNotify::OnMontageEnded);

	// Bind to montage notify begin
	AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this,&UPlayMontageAndWaitWithAnimNotify::OnAnimNotify);
};

void UPlayMontageAndWaitWithAnimNotify::ExternalCancel()
{
	Super::ExternalCancel();
	EndTask();
}

void UPlayMontageAndWaitWithAnimNotify::OnDestroy(bool bInOwnerFinished)
{
	Super::OnDestroy(bInOwnerFinished);

	// Clean up montage and notify bindings
	if (ACharacter* Character = Cast<ACharacter>(GetAvatarActor()))
	{
		if (UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance())
		{
			AnimInstance->OnMontageBlendingOut.RemoveDynamic(this, &UPlayMontageAndWaitWithAnimNotify::OnMontageBlendingOut);
			AnimInstance->OnMontageEnded.RemoveDynamic(this,&UPlayMontageAndWaitWithAnimNotify::OnMontageEnded);
		}

		if (AbilitySystemComponent.IsValid())
		{
			AbilitySystemComponent->CurrentMontageStop();
		}
	}

	Super::OnDestroy(bInOwnerFinished);
}

void UPlayMontageAndWaitWithAnimNotify::OnMontageBlendingOut(UAnimMontage* Montage, bool bInterrupted)
{
	OnMontageBlendedOut.Broadcast();
	EndTask();
}

void UPlayMontageAndWaitWithAnimNotify::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	OnMontageEndedDelegate.Broadcast();
	EndTask();
}

void UPlayMontageAndWaitWithAnimNotify::OnAnimNotify(FName AnimNotifyName,
                                                     const FBranchingPointNotifyPayload& BranchingPointPayload)
{
	OnAnimNotifyTriggered.Broadcast(AnimNotifyName);
}

