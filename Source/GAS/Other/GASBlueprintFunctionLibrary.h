// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GASBlueprintFunctionLibrary.generated.h"

class UCharacterInfoDataAsset;
/**
 * 
 */
UCLASS()
class GAS_API UGASBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
		static UCharacterInfoDataAsset* GetCharacterDataInfoAsset(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable)
		static float GetDirectionToTargetInDegress(const FVector& ActorForwardVector,const FVector& DirectionToTarget);
};
