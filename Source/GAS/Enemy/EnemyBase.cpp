#include "EnemyBase.h"
#include "GAS/AbilitySystemComponent/GASAbilitySystemComponent.h"
#include "GAS/DataAssets/CharacterInfoDataAsset.h"
#include "GAS/Other/GASBlueprintFunctionLibrary.h"

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
		InitializeCharacterInfo();
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
			OnArmorChanged(Data.NewValue,GASAttributeSet->GetMaxArmor());
		});
}

void AEnemyBase::InitializeCharacterInfo()
{
	if(IsValid(GASAbilitySystemComponent) && IsValid(GASAttributeSet))
	{
		if(UCharacterInfoDataAsset* CharacterInfoDataAsset = UGASBlueprintFunctionLibrary::GetCharacterDataInfoAsset(this))
		{
			if(const FCharacterInfo* FoundCharacterInfo = CharacterInfoDataAsset->CharacterInfo.Find(CharacterTag))
			{
				GASAbilitySystemComponent->AddCharacterAbilities(FoundCharacterInfo->DefaultAbilityClasses);
				GASAbilitySystemComponent->AddCharacterAbilitiesAndActivate(FoundCharacterInfo->DefaultAutoActivatedAbilityClasses);
				GASAbilitySystemComponent->AddDefaultGameplayEffects(FoundCharacterInfo->DefaultGameplayEffectClass);
			}
			else
			{
				GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Red,FString::Printf(TEXT("No Character Tag Selected For Actor: %s"),*GetNameSafe(this)));
			}
		}
	}
}
