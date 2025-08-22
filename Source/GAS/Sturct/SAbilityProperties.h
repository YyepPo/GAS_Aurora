#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "SAbilityProperties.generated.h"


USTRUCT(BlueprintType)
struct FAbilityProperties
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FGameplayTag AbilityTag;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
		FText InputKeyText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
		UTexture2D* AbilityIcon;
};

