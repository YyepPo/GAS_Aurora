// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/Sturct/SMeleeBoxHitInfo.h"
#include "UObject/Object.h"
#include "MeleeHitBoxInfoWrapper.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class GAS_API UMeleeHitBoxInfoWrapper : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
		FMeleeBoxHitInfo MeleeBoxHitInfo;
};
