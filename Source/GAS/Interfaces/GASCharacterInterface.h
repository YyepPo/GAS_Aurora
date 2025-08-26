#pragma once

#include "CoreMinimal.h"
#include "ScalableFloat.h"
#include "UObject/Interface.h"
#include "GASCharacterInterface.generated.h"

UINTERFACE()
class UGASCharacterInterface : public UInterface
{
	GENERATED_BODY()
};

class GAS_API IGASCharacterInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintCallable,BlueprintNativeEvent)
		void Death();
	
	UFUNCTION(BlueprintCallable,BlueprintNativeEvent)
		void AddExperience(const FScalableFloat& Experience);
	UFUNCTION(BlueprintCallable,BlueprintNativeEvent)
		void AddHitActor(AActor* Actor);
};
