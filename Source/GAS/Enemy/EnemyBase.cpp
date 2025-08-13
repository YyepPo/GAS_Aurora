#include "EnemyBase.h"
#include "GAS/AbilitySystemComponent/GASAbilitySystemComponent.h"
AEnemyBase::AEnemyBase()

{
	PrimaryActorTick.bCanEverTick = false;

	bReplicates = true;

	GASAbilitySystemComponent = CreateDefaultSubobject<UGASAbilitySystemComponent>(TEXT("GAS Ability System Component"));
	GASAbilitySystemComponent->SetIsReplicated(true);
	GASAbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
	check(GASAbilitySystemComponent);

	// Attribute set is replicated by default
	GASAttributeSet = CreateDefaultSubobject<UGASAttributeSet>(TEXT("GAS Attribute Set"));
	check(GASAttributeSet);

	GetMesh()->SetSimulatePhysics(false);
}

void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();
	
	InitAbilityInfo();
}

UAbilitySystemComponent* AEnemyBase::GetAbilitySystemComponent() const
{
	return GASAbilitySystemComponent;
}

void AEnemyBase::InitAbilityInfo()
{
	if(IsValid(GASAbilitySystemComponent) && IsValid(GASAttributeSet))
	{
		if(HasAuthority())
		{
			GASAbilitySystemComponent->AddCharacterAbilities(DefaultAbilityClasses);
			GASAbilitySystemComponent->AddDefaultGameplayEffects(DefaultGameplayEffectClass);	
		}

		BindToAttributeCallbacks();
	}}

void AEnemyBase::BindToAttributeCallbacks()
{
	if(IsValid(GASAbilitySystemComponent) == false || IsValid(GASAttributeSet) == false)
	{
		return;
	}

	// On Health Value Changed
	GASAbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GASAttributeSet->GetHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnHealthChanged(Data.NewValue,GASAttributeSet->GetMaxHealth());
		});

	// On Armor Value Changed
	GASAbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GASAttributeSet->GetArmorAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnArmorChagned(Data.NewValue,GASAttributeSet->GetMaxArmor());
		});
}