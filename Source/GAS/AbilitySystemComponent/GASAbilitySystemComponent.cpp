#include "GASAbilitySystemComponent.h"
#include "GAS/Abilities/GASAbility.h"

UGASAbilitySystemComponent::UGASAbilitySystemComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UGASAbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& Abilities)
{
	for (const TSubclassOf<UGameplayAbility>& Ability : Abilities)
	{
		FGameplayAbilitySpec GameplayAbilitySpec = FGameplayAbilitySpec(Ability,1);
		if(const UGASAbility* GASAbility = Cast<UGASAbility>(GameplayAbilitySpec.Ability))
		{
			GameplayAbilitySpec.DynamicAbilityTags.AddTag(GASAbility->AbilityTag);
			GiveAbility(GameplayAbilitySpec);
		}
		else
		{
			UE_LOG(LogTemp, Warning,TEXT("Select ability is not type of UGASAbility"));
		}
	}
}

void UGASAbilitySystemComponent::AddCharacterAbilitiesAndActivate(
	const TArray<TSubclassOf<UGameplayAbility>>& Abilities)
{
	for (const TSubclassOf<UGameplayAbility>& Ability : Abilities)
	{
		FGameplayAbilitySpec GameplayAbilitySpec = FGameplayAbilitySpec(Ability,1);
		if(const UGASAbility* GASAbility = Cast<UGASAbility>(GameplayAbilitySpec.Ability))
		{
			GameplayAbilitySpec.DynamicAbilityTags.AddTag(GASAbility->AbilityTag);
			GiveAbility(GameplayAbilitySpec);
			const bool bactivated  = TryActivateAbility(GameplayAbilitySpec.Handle);
			if(bactivated)
			{
				UE_LOG(LogTemp, Warning, TEXT("activatedddd"));
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("activatedddd noooot"));	
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning,TEXT("Select ability is not type of UGASAbility"));
		}
	}
}

void UGASAbilitySystemComponent::AddCharacterPassiveAbilities(const TArray<TSubclassOf<UGameplayAbility>>& PassiveAbilities)
{
	for (const TSubclassOf<UGameplayAbility>& Ability : PassiveAbilities)
	{
		const FGameplayAbilitySpec GameplayAbilitySpec = FGameplayAbilitySpec(Ability,1);
		GiveAbility(GameplayAbilitySpec);
	}
}

void UGASAbilitySystemComponent::AddDefaultGameplayEffects(TSubclassOf<UGameplayEffect> DefaultAttributeEffect)
{
	checkf(DefaultAttributeEffect,TEXT("No DefaultAttributeEffect select Actor: %s"),*GetAvatarActor()->GetActorNameOrLabel());
	const FGameplayEffectContextHandle Handle = MakeEffectContext();
	const FGameplayEffectSpecHandle Spec = MakeOutgoingSpec(DefaultAttributeEffect,1.f,Handle);
	ApplyGameplayEffectSpecToSelf(*Spec.Data.Get());
}

void UGASAbilitySystemComponent::RemoveCharacterAllAbilities()
{
	TArray<FGameplayAbilitySpecHandle> AllAbilities;
	GetAllAbilities(AllAbilities);

	for (const FGameplayAbilitySpecHandle& Ability : AllAbilities)
	{
		ClearAbility(Ability);
	}
	// Or use ClearAllAbilities()
}

void UGASAbilitySystemComponent::RemoveCharacterAbility(const FGameplayAbilitySpecHandle& AbilitySpecHandle)
{
	if(AbilitySpecHandle.IsValid())
	{
		ClearAbility(AbilitySpecHandle);
	}
}

void UGASAbilitySystemComponent::OnAbilityInputPressed(FGameplayTag Tag)
{
	if(!Tag.IsValid())
	{
		return;
	}

	ABILITYLIST_SCOPE_LOCK();

	for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if(AbilitySpec.DynamicAbilityTags.HasTagExact(Tag))
		{
			if(AbilitySpec.IsActive() == false)
			{
				TryActivateAbility(AbilitySpec.Handle);
			}
			else
			{
				InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputPressed,
					AbilitySpec.Handle,
					AbilitySpec.ActivationInfo.GetActivationPredictionKey());
			}
		}
	}
}

void UGASAbilitySystemComponent::OnAbilityInputReleased(FGameplayTag Tag)
{
	if(!Tag.IsValid())
	{
		return;
	}
	
	ABILITYLIST_SCOPE_LOCK();

	for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if(AbilitySpec.DynamicAbilityTags.HasTagExact(Tag))
		{
			InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputReleased,
				AbilitySpec.Handle,
				AbilitySpec.ActivationInfo.GetActivationPredictionKey());
		}
	}
}

void UGASAbilitySystemComponent::ActivateAbilityByTag(FGameplayTag AbilityTag)
{
	if(AbilityTag.IsValid() == false)
	{
		return;
	}

	for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if(AbilitySpec.Ability->AbilityTags.HasTagExact(AbilityTag))
		{
			if(AbilitySpec.IsActive() == false)
			{
				TryActivateAbility(AbilitySpec.Handle);
			}
			else
			{
				InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputPressed,
					AbilitySpec.Handle,
					AbilitySpec.ActivationInfo.GetActivationPredictionKey());
			}
		}
	}
}
