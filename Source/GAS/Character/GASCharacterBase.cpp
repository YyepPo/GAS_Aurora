#include "GASCharacterBase.h"

#include "GAS/AbilitySystemComponent/GASAbilitySystemComponent.h"
#include "Net/UnrealNetwork.h"

AGASCharacterBase::AGASCharacterBase(const FObjectInitializer& ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

	// Create and replicate ability system component
	GASAbilitySystemComponent = CreateDefaultSubobject<UGASAbilitySystemComponent>(TEXT("GAS Ability System Component"));
	GASAbilitySystemComponent->SetIsReplicated(true);
	GASAbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	check(GASAbilitySystemComponent);
	
	bIsDead = false;
}

void AGASCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AGASCharacterBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AGASCharacterBase,bIsDead);
}

void AGASCharacterBase::InitAbilityInfo()
{
}

void AGASCharacterBase::BindToAttributeCallbacks()
{
}

UAbilitySystemComponent* AGASCharacterBase::GetAbilitySystemComponent() const
{
	return GASAbilitySystemComponent;
}

void AGASCharacterBase::Death_Implementation()
{
	
}

void AGASCharacterBase::AddExperience_Implementation(const FScalableFloat& Experience)
{
	
}

void AGASCharacterBase::AddHitActor_Implementation(AActor* Actor)
{
	
}

void AGASCharacterBase::DisplayDamageIndicator_Implementation(const float DamageAmount, const FVector& TargetLocation)
{

}

void AGASCharacterBase::Server_SetIsDead_Implementation()
{
	bIsDead = true;
	OnRep_IsDead();

	const FGameplayTag DeathAbilityTag = FGameplayTag::RequestGameplayTag("Ability.Death");
    if(DeathAbilityTag.IsValid())
    {
    	const bool bActivated = GASAbilitySystemComponent->TryActivateAbilityByClass(DeathAbilityClass);
    	if(bActivated)
    	{
    		UE_LOG(LogTemp, Warning, TEXT("Ability: Death Activated"));
    	}
	    else
	    {
	    	UE_LOG(LogTemp, Warning, TEXT("Ability: Failed to activate"));
	    }
    }
    else
    {
	    UE_LOG(LogTemp, Warning, TEXT("Ability: Death ability tag is not valid"));
    }
}

void AGASCharacterBase::OnRep_IsDead()
{
	OnDeath();
}

