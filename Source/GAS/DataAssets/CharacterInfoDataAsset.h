#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Engine/DataAsset.h"
#include "CharacterInfoDataAsset.generated.h"

USTRUCT()
struct FCharacterInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
		TArray<TSubclassOf<UGameplayAbility>> DefaultAbilityClasses;
	// These abilities will be activated from start
	UPROPERTY(EditAnywhere)
		TArray<TSubclassOf<UGameplayAbility>> DefaultAutoActivatedAbilityClasses;
	UPROPERTY(EditAnywhere)
		TSubclassOf<UGameplayEffect> DefaultGameplayEffectClass;
};

UCLASS()
class GAS_API UCharacterInfoDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere)
		TMap<FGameplayTag,FCharacterInfo> CharacterInfo;
};
