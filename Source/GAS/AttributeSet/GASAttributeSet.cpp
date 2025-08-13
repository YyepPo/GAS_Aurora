#include "GASAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "Net/UnrealNetwork.h"

void UGASAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
}

void UGASAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
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
		}
	}

	if(Data.EvaluatedData.Attribute == GetHealthAttribute() && GetArmor() == 0)
	{
		SetHealth(FMath::Clamp(GetHealth(),0,GetMaxHealth()));
	}

	if(Data.EvaluatedData.Attribute == GetManaAttribute())
	{
		SetMana(FMath::Clamp(GetMana(),0,GetMaxMana()));
	}

	if(Data.EvaluatedData.Attribute == GetArmorAttribute())
	{
		SetArmor(FMath::Clamp(GetArmor(),0,GetMaxArmor()));
	}
}

void UGASAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UGASAttributeSet,Health,COND_None,REPNOTIFY_OnChanged);
	DOREPLIFETIME_CONDITION_NOTIFY(UGASAttributeSet,MaxHealth,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGASAttributeSet,Mana,COND_None,REPNOTIFY_OnChanged);
	DOREPLIFETIME_CONDITION_NOTIFY(UGASAttributeSet,MaxMana,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGASAttributeSet,Armor,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGASAttributeSet,MaxArmor,COND_None,REPNOTIFY_Always);
	
}

void UGASAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGASAttributeSet,Health,OldHealth);
}

void UGASAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGASAttributeSet,MaxHealth,OldMaxHealth);
}

void UGASAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGASAttributeSet,Mana,OldMana);
}

void UGASAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGASAttributeSet,MaxMana,OldMaxMana);
}

void UGASAttributeSet::OnRep_Armor(const FGameplayAttributeData& OldArmor)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGASAttributeSet,Armor,OldArmor);
}

void UGASAttributeSet::OnRep_MaxArmor(const FGameplayAttributeData& OldMaxArmor)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGASAttributeSet,MaxArmor,OldMaxArmor)
}
