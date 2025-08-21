#include "GASPlayerState.h"

#include "AttributeSet/GASHealthAttributeSet.h"

AGASPlayerState::AGASPlayerState()
{
	NetUpdateFrequency = 100.f;
	MinNetUpdateFrequency = 61.f;

	AbilitySystemComponent = CreateDefaultSubobject<UGASAbilitySystemComponent>(TEXT("Ability System Component"));
	AbilitySystemComponent->SetIsReplicated(true);
	// Only replicate minimal gameplay effect info to simulated proxies but full info to owners and autonomous proxies
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UGASAttributeSet>(TEXT("Attribute Set"));
	HealthAttributeSet = CreateDefaultSubobject<UGASHealthAttributeSet>(TEXT("Health Attribute Set"));
}

void AGASPlayerState::BeginPlay()
{
	Super::BeginPlay();

	if(IsValid(AbilitySystemComponent) && IsValid(AttributeSet) && IsValid(HealthAttributeSet))
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(HealthAttributeSet->GetHealthAttribute()).AddLambda(
			[this] (const FOnAttributeChangeData& Data)
			{
				OnHealthValueChanged.Broadcast(Data.NewValue,HealthAttributeSet->GetMaxHealth());	
			});

		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetManaAttribute()).AddLambda(
			[this] (const FOnAttributeChangeData& Data)
			{
				OnManaValueChanged.Broadcast(Data.NewValue,AttributeSet->GetMaxMana());	
			});
	}
}

UAbilitySystemComponent* AGASPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
