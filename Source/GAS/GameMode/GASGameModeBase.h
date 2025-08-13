#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "GASGameModeBase.generated.h"

UCLASS()
class GAS_API AGASGameModeBase : public AGameMode
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly,Category = "GAS|DataAsset")
		class UCharacterInfoDataAsset* CharacterInfoDataAsset;
};
