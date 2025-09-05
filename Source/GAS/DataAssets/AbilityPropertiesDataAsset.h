#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GAS/Sturct/SAbilityProperties.h"
#include "AbilityPropertiesDataAsset.generated.h"

UCLASS()
class GAS_API UAbilityPropertiesDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
		FAbilityProperties AbilityProperties;
};
