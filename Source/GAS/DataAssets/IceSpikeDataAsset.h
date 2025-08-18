// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "IceSpikeDataAsset.generated.h"

USTRUCT()
struct FIceSpikeProperties
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
		UAnimMontage* MontageToPlay;
	// The number of spikes that are going to be spawned
	UPROPERTY(EditAnywhere)
		int32 NumberOfSpikes;
	UPROPERTY(EditAnywhere)
		float SphereRadius;
	UPROPERTY(EditAnywhere)
		float EmitterScaleIncrement;
	// Distance (in cm) to trace downward when aligning the VFX to the ground.Ensures the effect doesn't float if the terrain is uneven.
	UPROPERTY(EditAnywhere)
		float GroundTraceDistance = 75.f;

	FIceSpikeProperties(): MontageToPlay(nullptr),NumberOfSpikes(10), SphereRadius(300.f), EmitterScaleIncrement(0.1f){}
};

UCLASS()
class GAS_API UIceSpikeDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
		FIceSpikeProperties IceSpikeProperties;
};
