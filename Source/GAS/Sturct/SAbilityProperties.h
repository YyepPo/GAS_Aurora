#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "SAbilityProperties.generated.h"


USTRUCT(BlueprintType)
struct FAbilityProperties
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FText Name;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FText Description;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
		FText InputKeyAsText;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
		TObjectPtr<UTexture2D> Icon;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FGameplayTag AbilityTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float Cooldown;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float RequiredMana;

	bool operator==(const FAbilityProperties& Other) const
	{
		return Name.EqualTo(Other.Name) &&
			   Description.EqualTo(Other.Description) &&
			   InputKeyAsText.EqualTo(Other.InputKeyAsText) &&
			   Icon == Other.Icon &&
			   AbilityTag == Other.AbilityTag &&
			   FMath::IsNearlyEqual(Cooldown, Other.Cooldown) &&
			   FMath::IsNearlyEqual(RequiredMana, Other.RequiredMana);
	}
};

