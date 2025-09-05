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

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
		FText Name;
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
		FText Description;
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
		FGameplayTag ItemTag;
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
		TObjectPtr<UTexture2D> Icon;
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
		bool bConsumable = false;
	// If consumable is true, then  the gameplay effect is usable 
	UPROPERTY(EditAnywhere, meta = (EditCondition = "bConsumable"))
		TSubclassOf<UGameplayEffect> GameplayEffect;
	
};
