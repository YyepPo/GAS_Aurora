
#include "GASHealthAttributeSet.h"
#include "GameplayEffectExtension.h"
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
}

void UGASHealthAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

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
			if(GetHealth() == 0)
			{
				if(AActor* TargetActor = Data.Target.GetAvatarActor())
				{
					if(TargetActor->Implements<UGASCharacterInterface>())
					{
						IGASCharacterInterface::Execute_Death(TargetActor);
						UE_LOG(LogTemp, Warning, TEXT("Death: Interface is valid in tartget actor"));
					}
					else
					{
						UE_LOG(LogTemp, Warning, TEXT("Death: Interface not valid in target actor"));
					}
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("Death: Target actor not valid"));
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
