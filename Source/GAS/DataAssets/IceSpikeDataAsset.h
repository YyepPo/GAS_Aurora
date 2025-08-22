// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GAS/Sturct/SAbilityProperties.h"
#include "IceSpikeDataAsset.generated.h"

USTRUCT()
struct FIceSpikeProperties : public FAbilityProperties
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Ice Spike Properties")
		UAnimMontage* MontageToPlay;
	// The number of spikes that are going to be spawned
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Ice Spike Properties")
		int32 NumberOfSpikes;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Ice Spike Properties")
		float SphereRadius;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Ice Spike Properties")
		float EmitterScaleIncrement;
	// Distance (in cm) to trace downward when aligning the VFX to the ground.Ensures the effect doesn't float if the terrain is uneven.
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Ice Spike Properties")
		float GroundTraceDistance = 75.f;

	FIceSpikeProperties(): MontageToPlay(nullptr),NumberOfSpikes(10), SphereRadius(300.f), EmitterScaleIncrement(0.1f){}
};

UCLASS()
class GAS_API UIceSpikeDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
		FIceSpikeProperties IceSpikeProperties;
};
