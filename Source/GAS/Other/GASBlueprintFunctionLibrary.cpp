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
