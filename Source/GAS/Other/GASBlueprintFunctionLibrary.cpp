// Fill out your copyright notice in the Description page of Project Settings.


#include "GASBlueprintFunctionLibrary.h"
#include "GAS/GameMode/GASGameModeBase.h"
#include "Kismet/GameplayStatics.h"

UCharacterInfoDataAsset* UGASBlueprintFunctionLibrary::GetCharacterDataInfoAsset(const UObject* WorldContextObject)
{
	if(AGASGameModeBase* GameMode = Cast<AGASGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject)))
	{
		return  GameMode->CharacterInfoDataAsset;
	}
	
	return nullptr;
}

float UGASBlueprintFunctionLibrary::GetDirectionToTargetInDegress(const FVector& ActorForwardVector,
	const FVector& DirectionToTarget)
{
	float DotProduct =	FVector::DotProduct(ActorForwardVector,DirectionToTarget);

	float Acos = FMath::Acos(DotProduct);
	Acos = FMath::RadiansToDegrees(Acos);

	FVector CrossProduct = FVector::CrossProduct(ActorForwardVector, DirectionToTarget);
	(CrossProduct.Z < 0) ? Acos *= -1 : Acos *= 1;

	return Acos;
}
