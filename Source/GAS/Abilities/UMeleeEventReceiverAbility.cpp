#include "UMeleeEventReceiverAbility.h"

#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "GAS/UObjects/MeleeHitBoxInfoWrapper.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GAS/AttributeSet/GASAttributeSet.h"
#include "GAS/Other/GASBlueprintFunctionLibrary.h"

UUMeleeEventReceiverAbility::UUMeleeEventReceiverAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UUMeleeEventReceiverAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	ASC = ActorInfo->AbilitySystemComponent.Get();
	if(IsValid(ASC) == false)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	OwnerCharacter = Cast<ACharacter>(ASC->GetAvatarActor());
	if(IsValid(OwnerCharacter) == false)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	FGameplayTag EventTag = FGameplayTag::RequestGameplayTag("Event.Melee.Hit");
	if (!EventTag.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid gameplay tag: Event.Melee.Hit"));
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	UAbilityTask_WaitGameplayEvent* WaitTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, EventTag, nullptr, false, false);
	if (WaitTask)
	{
		WaitTask->EventReceived.AddDynamic(this, &UUMeleeEventReceiverAbility::OnGameplayEventReceived);
		WaitTask->ReadyForActivation();
	}
	else
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
	}
}

void UUMeleeEventReceiverAbility::OnGameplayEventReceived(FGameplayEventData Payload)
{
	const AActor* Instigator = Payload.Instigator.Get();
	const UObject* Test =	Payload.OptionalObject;
	if(Instigator == nullptr || Test == nullptr)
	{
		return;
	}

	const UMeleeHitBoxInfoWrapper* HitBoxInfoWrapper = Cast<UMeleeHitBoxInfoWrapper>(Test);
	if(HitBoxInfoWrapper == nullptr)
	{
		return;
	}

	const FVector ActorForwardVector = OwnerCharacter->GetActorForwardVector();
	const FVector DirectionToTarget = (Instigator->GetActorLocation() - OwnerCharacter->GetActorLocation()).GetSafeNormal();
	const float Acos = UGASBlueprintFunctionLibrary::GetDirectionToTargetInDegress(ActorForwardVector,DirectionToTarget);
	 
	PlayMontageBasedOnDirection(Acos);
	ApplyForceToCharacterBasedOnDirection(DirectionToTarget * -HitBoxInfoWrapper->MeleeBoxHitInfo.ImpulseForce);

	// Execute Hit Impact Cue
	FGameplayTag HitImpactCueTag = FGameplayTag::RequestGameplayTag("GameplayCue.Enemy.Minion.HitImpact");
	FGameplayCueParameters GameplayCueParameter;
	GameplayCueParameter.Location = HitBoxInfoWrapper->MeleeBoxHitInfo.HitImpactLocation;
	GameplayCueParameter.Instigator = GetAvatarActorFromActorInfo();
	GetAbilitySystemComponentFromActorInfo()->ExecuteGameplayCue(HitImpactCueTag,GameplayCueParameter);
}

void UUMeleeEventReceiverAbility::PlayMontageBasedOnDirection(float Angle)
{
	UAnimMontage* SelectedMontage = nullptr;
	if (Angle >= -45.f && Angle < 45.f)
	{
		SelectedMontage = ReactFrontMontage;
	}
	else if (Angle >= -135.f && Angle < -45.f)
	{
		SelectedMontage = ReactLeftMontage; 
	}
	else if (Angle >= 45.f && Angle < 135.f)
	{
		SelectedMontage = ReactRightMontage;
	}
	else
	{
		SelectedMontage = ReactBackMontage;
	}

	if (SelectedMontage)
	{
		UAbilityTask_PlayMontageAndWait* PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
			this, NAME_None, SelectedMontage);
		if (PlayMontageTask)
		{
			//PlayMontageTask->OnCompleted.AddDynamic(this, &UUMeleeEventReceiverAbility::OnMontageFinished);
			//PlayMontageTask->OnInterrupted.AddDynamic(this, &UUMeleeEventReceiverAbility::OnMontageFinished);
			//PlayMontageTask->OnCancelled.AddDynamic(this, &UUMeleeEventReceiverAbility::OnMontageFinished);
			
			PlayMontageTask->ReadyForActivation();
		}
	}
}

void UUMeleeEventReceiverAbility::ApplyForceToCharacterBasedOnDirection(const FVector& Direction) const
{
	UCharacterMovementComponent* MovementComponent = OwnerCharacter->GetCharacterMovement();
	if (MovementComponent == nullptr)
	{
		return;
	}
	
	MovementComponent->AddImpulse(Direction, true);
}
