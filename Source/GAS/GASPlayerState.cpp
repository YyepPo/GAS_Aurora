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

	CurrentLevel = 1;
	CurrentExperience = 0;
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

void AGASPlayerState::AddXP(const FScalableFloat& Experience)
{
	if(RequiredExperienceToLevelUp.IsValid() == false)
	{
		GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Red,FString::Printf(TEXT("GAS PlayerState: RequiredExperienceToLevelUp curve data table is not select.")));
		return;
	}
	
	CurrentExperience += Experience.GetValueAtLevel(CurrentLevel);
	UE_LOG(LogTemp, Warning, TEXT("PlayerState: Player %s Gained = %f XP"),*GetPawn()->GetActorNameOrLabel(),Experience.GetValueAtLevel(CurrentLevel));
	UE_LOG(LogTemp, Warning, TEXT("PlayerState: Player's %s CurrentXP = %d"),*GetPawn()->GetActorNameOrLabel(),CurrentExperience);
	UE_LOG(LogTemp, Warning, TEXT("PlayerState: Player's %s Current Level = %d"),*GetPawn()->GetActorNameOrLabel(),CurrentLevel);

	const int32 RequiredXP = RequiredExperienceToLevelUp.GetValueAtLevel(CurrentLevel);
	if(CurrentExperience >= RequiredXP)
	{
		++CurrentLevel;
		UE_LOG(LogTemp, Warning, TEXT("PlayerState: Level Up->Current Level = %d"),CurrentLevel);
		CurrentExperience -= RequiredXP;
		OnLevelUp();
	}
}

void AGASPlayerState::OnLevelUp() const
{
	if(IsValid(AbilitySystemComponent) == false)
	{
		return;
	}

	if(IsValid(LevelUpGameplayEffectClass) == false)
	{
		GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Red,FString::Printf(TEXT("Player State: LevelUpGameplayEffect class is not selected.")));
		return;
	}

	FGameplayEffectContextHandle ContextHandle = AbilitySystemComponent->MakeEffectContext();
	FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(LevelUpGameplayEffectClass,CurrentLevel,ContextHandle);
	AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
}
