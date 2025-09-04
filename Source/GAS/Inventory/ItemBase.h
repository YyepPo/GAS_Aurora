// MyDataTableRow.h
#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "Engine/DataTable.h"
#include "ItemBase.generated.h"

USTRUCT(BlueprintType)
struct FItemBase : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
		FText Name;
	UPROPERTY(EditAnywhere)
		FText Description;
	UPROPERTY(EditAnywhere)
		FGameplayTag ItemTag;
	UPROPERTY(EditAnywhere)
		TObjectPtr<UTexture2D> Icon;
	UPROPERTY(EditAnywhere)
		bool bConsumable = false;
	// If consumable is true, then  the gameplay effect is usable 
	UPROPERTY(EditAnywhere, meta = (EditCondition = "bConsumable"))
		TSubclassOf<UGameplayEffect> GameplayEffect;
	
};
