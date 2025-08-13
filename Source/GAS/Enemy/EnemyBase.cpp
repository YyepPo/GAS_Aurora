#include "EnemyBase.h"
#include "GAS/AbilitySystemComponent/GASAbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
AEnemyBase::AEnemyBase()
{
	PrimaryActorTick.bCanEverTick = false;

	GASAbilitySystemComponent = CreateDefaultSubobject<UGASAbilitySystemComponent>(TEXT("GAS Ability System Component"));
	GASAbilitySystemComponent->SetIsReplicated(true);
	GASAbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	check(GASAbilitySystemComponent);

	// Attribute set is replicated by default
	GASAttributeSet = CreateDefaultSubobject<UGASAttributeSet>(TEXT("GAS Attribute Set"));
	check(GASAttributeSet);

	GetMesh()->SetSimulatePhysics(false);
}

void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();

	if(IsValid(GASAbilitySystemComponent) && IsValid(GASAttributeSet))
	{
		if(HasAuthority())
		{
			GASAbilitySystemComponent->AddCharacterAbilities(DefaultAbilityClasses);
			GASAbilitySystemComponent->AddDefaultGameplayEffects(DefaultGameplayEffectClass);
		}
	}
}

UAbilitySystemComponent* AEnemyBase::GetAbilitySystemComponent() const
{
	return GASAbilitySystemComponent;
}


