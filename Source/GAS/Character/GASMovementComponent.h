#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GASMovementComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GAS_API UGASMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UGASMovementComponent();
	
	virtual void SimulateMovement(float DeltaTime) override;

	void SetReplicatedAcceleration(const FVector& InAcceleration);
	
protected:

	UPROPERTY(Transient)
    	bool bHasReplicatedAcceleration = false;
};
