// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

USTRUCT()
struct FInventoryPackage
{
	virtual bool NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess);
	
	GENERATED_BODY()

	virtual ~FInventoryPackage() = default;
	
	UPROPERTY()
		TArray<FGameplayTag> ItemTags;
	UPROPERTY()
		TArray<int32> Quantities;
};
template<>
struct TStructOpsTypeTraits<FInventoryPackage> : public TStructOpsTypeTraitsBase2<FInventoryPackage>
{
	enum
	{
		WithNetSerializer = true
	};
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAS_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UInventoryComponent();
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
		void AddItem(FGameplayTag ItemTag,const int32 Quantity = 1);
	UFUNCTION(BlueprintCallable, Category = "Inventory")
		void UseItem(FGameplayTag ItemTag);
	
private:

	UFUNCTION(Server,Reliable)
		void Server_AddItem(FGameplayTag ItemTag,const int32 Quantity);
	UFUNCTION(Server,Reliable)
		void Server_UseItem(FGameplayTag ItemTag);
	
	UPROPERTY()
		TMap<FGameplayTag,int32> InventoryMap;
	
	UPROPERTY(ReplicatedUsing = OnRep_CachedInventory)
		FInventoryPackage CachedInventory;
	UFUNCTION()
		void OnRep_CachedInventory();

	void PackageInventoryMap(FInventoryPackage& OutInventoryPackage);
	void ReconstructInventoryMap(const FInventoryPackage& InInventoryPackage);

	UPROPERTY(EditDefaultsOnly)
		UDataTable* ItemDataTable;
};
	
