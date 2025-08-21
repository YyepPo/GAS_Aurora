#include "EnemyBase.h"

#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "GAS/AbilitySystemComponent/GASAbilitySystemComponent.h"
#include "GAS/DataAssets/CharacterInfoDataAsset.h"
#include "GAS/Other/GASBlueprintFunctionLibrary.h"

AEnemyBase::AEnemyBase()
{
	PrimaryActorTick.bCanEverTick = false;

	// Replicate this actor
	bReplicates = true;

	// Create Health Attribute Set
	HealthAttributeSet = CreateDefaultSubobject<UGASHealthAttributeSet>(TEXT("GAS Attribute Set"));
	check(HealthAttributeSet);

	// Create Overhead widget component that is going to display health & mana bar
	OverheadWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("Overhead Widget Component"));
	OverheadWidgetComponent->SetupAttachment(GetRootComponent());

	GetMesh()->SetSimulatePhysics(false);
}

void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();
	
	InitAbilityInfo();
}

void AEnemyBase::InitAbilityInfo()
{
	if(IsValid(GASAbilitySystemComponent) && IsValid(HealthAttributeSet))
	{
		InitializeCharacterInfo();
		BindToAttributeCallbacks();
	}}

void AEnemyBase::BindToAttributeCallbacks()
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

	// On Armor Value Changed
	GASAbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(HealthAttributeSet->GetArmorAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnArmorChanged(Data.NewValue,HealthAttributeSet->GetMaxArmor());
		});
}

void AEnemyBase::Death_Implementation()
{
	Server_SetIsDead();

	if(HasAuthority())
	{
		FTimerHandle DeathTimerHandle;
            GetWorld()->GetTimerManager().SetTimer(DeathTimerHandle, [this]()
            {
                Destroy();
            }, 1.0f, false);
	}
}

void AEnemyBase::OnDeath()
{
	GetMesh()->SetVisibility(false);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_GameTraceChannel2,ECR_Ignore);
	if(IsValid(OverheadWidgetComponent))
	{
		OverheadWidgetComponent->DestroyComponent();
	}
}

void AEnemyBase::InitializeCharacterInfo()
{
	if(IsValid(GASAbilitySystemComponent) && IsValid(HealthAttributeSet) && HasAuthority())
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
