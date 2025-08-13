#pragma once

#include "CoreMinimal.h"
#include "SMeleeBoxHitInfo.generated.h"

USTRUCT(BlueprintType)
struct FMeleeBoxHitInfo
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector HitBoxLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float ImpulseForce;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float HitBoxRadius;
};

