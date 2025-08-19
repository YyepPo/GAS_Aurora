#include "AuroraDecoy.h"

#include "GAS/DataAssets/CharacterInfoDataAsset.h"
#include "GAS/Other/GASBlueprintFunctionLibrary.h"

AAuroraDecoy::AAuroraDecoy()
{
	PrimaryActorTick.bCanEverTick = false;

	bReplicates = true;
	NetUpdateFrequency = 10.f;

	// Initialize GAS Ability System Component
	GASAbilitySystemComponent = CreateDefaultSubobject<UGASAbilitySystemComponent>(TEXT("GAS Ability System Component"));
	GASAbilitySystemComponent->SetIsReplicated(true);
	GASAbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
	check(GASAbilitySystemComponent);

	// Initialize Health Attribute Set
	HealthAttributeSet = CreateDefaultSubobject<UHealthAttributeSet>(TEXT("Health Attribute Set"));
	check(HealthAttributeSet);
}

void AAuroraDecoy::BeginPlay()
{
	Super::BeginPlay();

	InitGameplayEffect();
	BindToAttributeCallbacks();
}

void AAuroraDecoy::InitGameplayEffect()
{
	if(IsValid(GASAbilitySystemComponent) == false || IsValid(HealthAttributeSet) == false)
	{
		return;
	}
	
	if(UCharacterInfoDataAsset* CharacterInfoDataAsset = UGASBlueprintFunctionLibrary::GetCharacterDataInfoAsset(this))
	{
		if(const FCharacterInfo* FoundCharacterInfo = CharacterInfoDataAsset->CharacterInfo.Find(CharacterTag))
		{
			GASAbilitySystemComponent->AddDefaultGameplayEffects(FoundCharacterInfo->DefaultGameplayEffectClass);
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Red,FString::Printf(TEXT("No Character Tag Selected For Actor: %s"),*GetNameSafe(this)));
		}
	}
}

void AAuroraDecoy::BindToAttributeCallbacks()
{
	if(IsValid(GASAbilitySystemComponent) == false || IsValid(HealthAttributeSet) == false)
	{
		return;
	}

	// On Health Value Changed
	GASAbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(HealthAttributeSet->GetHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnHealthChanged(Data.NewValue,HealthAttributeSet->GetMaxHealth());
		});

}
