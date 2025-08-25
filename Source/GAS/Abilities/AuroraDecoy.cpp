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
	GASAbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	check(GASAbilitySystemComponent);

	GASAttributeSet = CreateDefaultSubobject<UGASHealthAttributeSet>(TEXT("Health Attribute Set"));
	check(GASAttributeSet);
}

void AAuroraDecoy::BeginPlay()
{
	Super::BeginPlay();
}

void AAuroraDecoy::InitGameplayEffect()
{
	if(IsValid(GASAbilitySystemComponent) == false)// || IsValid(HealthAttributeSet) == false)
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
	if(IsValid(GASAbilitySystemComponent) == false)
	{
		GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Red,FString::Printf(TEXT("Aurora Decoy: GASAbilitySystemComponent is not valid")));
		return;
	}
	
	if(IsValid(GASAttributeSet) == false)
	{
		GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Red,FString::Printf(TEXT("Aurora Decoy: GASAttributeSet is not valid")));
		return;
	}
	
	// On Health Value Changed
	GASAbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GASAttributeSet->GetHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnHealthChanged(Data.NewValue,GASAttributeSet->GetMaxHealth());
		});

}
