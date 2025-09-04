#include "GASPlayerController.h"

#include "GAS/GASPlayerState.h"
#include "GAS/AbilitySystemComponent/GASAbilitySystemComponent.h"
#include "GAS/InputComponent/GASEnhancedInputComponent.h"
#include "AbilitySystemBlueprintLibrary.h"

AGASPlayerController::AGASPlayerController()
{
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("Inventory Component"));
	InventoryComponent->SetIsReplicated(true);
	checkf(InventoryComponent, TEXT("Inventory Component not found"));
}

void AGASPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if(AGASPlayerState* GASPlayerState = GetPlayerState<AGASPlayerState>())
	{
		GASAbilitySystemComponent =	GASPlayerState->GetAbilitySystemComp();
	}
}

void AGASPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if(UGASEnhancedInputComponent* GASInputComp = Cast<UGASEnhancedInputComponent>(InputComponent))
	{
		GASInputComp->BindAbilityActions(InputConfig,this,&AGASPlayerController::OnAbilityPressed,&AGASPlayerController::OnAbilityReleased);
	}
}

UAbilitySystemComponent* AGASPlayerController::GetAbilitySystemComponent() const
{
	return UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn());
}

void AGASPlayerController::OnAbilityPressed(FGameplayTag Tag)
{
	if(!IsValid(GetGASAbilitySystemComponent()))
	{
		return;
	}

	GASAbilitySystemComponent->OnAbilityInputPressed(Tag);
}

void AGASPlayerController::OnAbilityReleased(FGameplayTag Tag)
{
	if(!IsValid(GetGASAbilitySystemComponent()))
	{
		return;
	}

	GASAbilitySystemComponent->OnAbilityInputReleased(Tag);
}

TObjectPtr<UGASAbilitySystemComponent> AGASPlayerController::GetGASAbilitySystemComponent()
{
	if(!IsValid(GASAbilitySystemComponent))
	{
		if(AGASPlayerState* GASPlayerState = GetPlayerState<AGASPlayerState>())
		{
			GASAbilitySystemComponent =	GASPlayerState->GetAbilitySystemComp();
			return GASAbilitySystemComponent;
		}
	}
	
	return GASAbilitySystemComponent;
}
