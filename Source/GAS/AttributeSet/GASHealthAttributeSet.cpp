
#include "GASHealthAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "GAS/Character/GASCharacterBase.h"
#include "GAS/Interfaces/GASCharacterInterface.h"
#include "Net/UnrealNetwork.h"

void UGASHealthAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION_NOTIFY(UGASHealthAttributeSet,Health,COND_None,REPNOTIFY_OnChanged);
	DOREPLIFETIME_CONDITION_NOTIFY(UGASHealthAttributeSet,MaxHealth,COND_None,REPNOTIFY_OnChanged);
	DOREPLIFETIME_CONDITION_NOTIFY(UGASHealthAttributeSet,Armor,COND_None,REPNOTIFY_OnChanged);
	DOREPLIFETIME_CONDITION_NOTIFY(UGASHealthAttributeSet,MaxArmor,COND_None,REPNOTIFY_OnChanged);
}

void UGASHealthAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetMaxHealthAttribute())
	{
		PreviousMaxHealth = MaxHealth.GetCurrentValue();
	}
}

void UGASHealthAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	FGameplayEffectContextHandle ContextHandle = Data.EffectSpec.GetContext();
	
	if(Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		const float LocalDamageDone = GetDamage();
		SetDamage(0.f);

		const float NewArmor = GetArmor() - LocalDamageDone;
		SetArmor(FMath::Clamp(NewArmor,0,GetMaxArmor()));

		if(GetArmor() == 0)
		{
			const float NewHealth = GetHealth() - LocalDamageDone;
			SetHealth(FMath::Clamp(NewHealth, 0.0f, GetMaxHealth()));
			
			// On Death
			if(GetHealth() == 0)
			{
				AActor* TargetActor = Data.Target.GetAvatarActor();
				if(IsValid(TargetActor) == false)
				{
					return;
				}
				
				AActor* Instigator = ContextHandle.GetOriginalInstigatorAbilitySystemComponent()->GetAvatarActor();
				if(Instigator && Instigator->IsA(AGASCharacterBase::StaticClass()) && Instigator->Implements<UGASCharacterInterface>() && Instigator->HasAuthority())
				{
					IGASCharacterInterface::Execute_AddHitActor(TargetActor,Instigator);
					GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Yellow,FString::Printf(TEXT("Add hit actor interface function has benn called")));
				}

				// Execute Death interface function
				if(TargetActor->Implements<UGASCharacterInterface>())
				{
					IGASCharacterInterface::Execute_Death(TargetActor);
				}
			}
		}
	}

	if(Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(),0,GetMaxHealth()));
	}
	
	if(Data.EvaluatedData.Attribute == GetArmorAttribute())
	{
		SetArmor(FMath::Clamp(GetArmor(),0,GetMaxArmor()));
	}

	if (Data.EvaluatedData.Attribute == GetMaxHealthAttribute())
	{
		// Adjusts the current health proportionally when the max health attribute changes
		const float NewHealthValue = GetMaxHealth() * (GetHealth() / PreviousMaxHealth);
		SetHealth(FMath::Clamp(NewHealthValue,0,GetMaxHealth()));
	}
}

void UGASHealthAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGASHealthAttributeSet,Health,OldHealth);
}

void UGASHealthAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGASHealthAttributeSet,MaxHealth,OldMaxHealth);
}

void UGASHealthAttributeSet::OnRep_Armor(const FGameplayAttributeData& OldArmor)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGASHealthAttributeSet,Armor,OldArmor);
}

void UGASHealthAttributeSet::OnRep_MaxArmor(const FGameplayAttributeData& OldMaxArmor)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGASHealthAttributeSet,MaxArmor,OldMaxArmor);
}
