#include "GAS/Inventory/InventoryComponent.h"

#include "ItemBase.h"
#include "GAS/GASCharacter.h"
#include "GAS/PlayerController/GASPlayerController.h"
#include "Net/UnrealNetwork.h"

//Create Inventory Map,that is going to be used for server only and it will be reconstructed from CachedInventory
//Create CachedInventory Struct that is going to be replicated(on rep for clients receveing it), reason is that map is not replicated by default, and when a client recevies it, it will reconstruct inventorymap
//Create InventoryPackage Struct, with netseriliazed enabled.
//Add item,only by the server, if client, call server rpc to add an item, and update the map only on the server.
//Create packageInventory function, to prepare the data for network serilizatin and sending to network
// Create recounstructmap function, to reconstruct the map

bool FInventoryPackage::NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess)
{
	SafeNetSerializeTArray_WithNetSerialize<50>(Ar,ItemTags,Map);
	SafeNetSerializeTArray_Default<50>(Ar,Quantities);
	
	bOutSuccess = true;
	return true;
}

UInventoryComponent::UInventoryComponent()
{
	
}

void UInventoryComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UInventoryComponent,CachedInventory);
}

void UInventoryComponent::AddItem(FGameplayTag ItemTag, const int32 Quantity)
{
	AActor* Owner =	GetOwner();
	if (IsValid(Owner) == false)
	{
		return;
	}

	// Client side
	if (Owner->HasAuthority() == false)
	{
		Server_AddItem(ItemTag,Quantity);
		return;
	}

	// Server side
	// Update inventory with new item
	// If the item already exists in the map then increase it quantity otherwise add it as a new item
	if (InventoryMap.Contains(ItemTag))
	{
		InventoryMap[ItemTag] += Quantity;
	}
	else
	{
		InventoryMap.Emplace(ItemTag, Quantity);
	}

	GEngine->AddOnScreenDebugMessage(-1,
		5.f,
		FColor::Red,
		FString::Printf(TEXT("Server: Added Item: %s & quantity: %d"),*ItemTag.ToString(),Quantity));

	// Prepare the struct serialized struct to send to network
	PackageInventoryMap(CachedInventory);
}

void UInventoryComponent::Server_AddItem_Implementation(FGameplayTag ItemTag, const int32 Quantity)
{
	AddItem(ItemTag,Quantity);
}

void UInventoryComponent::OnRep_CachedInventory()
{
	ReconstructInventoryMap(CachedInventory);
}

void UInventoryComponent::PackageInventoryMap(FInventoryPackage& OutInventoryPackage)
{
	OutInventoryPackage.ItemTags.Empty();
	OutInventoryPackage.Quantities.Empty();

	for (auto& Element : InventoryMap)
	{
		OutInventoryPackage.ItemTags.Add(Element.Key);
		OutInventoryPackage.Quantities.Add(Element.Value);
	}
}

void UInventoryComponent::ReconstructInventoryMap(const FInventoryPackage& InInventoryPackage)
{
	InventoryMap.Empty();

	for (int i = 0; i < InInventoryPackage.ItemTags.Num(); i++)
	{
		InventoryMap.Emplace(InInventoryPackage.ItemTags[i], InInventoryPackage.Quantities[i]);
		GEngine->AddOnScreenDebugMessage(-1,
		5.f,
		FColor::Red,
		FString::Printf(TEXT("Client: Received Added Item: %s & quantity: %d"),*InInventoryPackage.ItemTags[i].ToString(),InInventoryPackage.Quantities[i]));
	}
}

void UInventoryComponent::UseItem(FGameplayTag ItemTag)
{
	AActor* Owner =	GetOwner();
	if (IsValid(Owner) == false)
	{
		return;
	}

	// Client Side
	if (Owner->HasAuthority() == false)
	{
		Server_UseItem(ItemTag);
		return;
	}

	// Server Side
	// Check if item exists on the inventory
	const bool ItemExists = InventoryMap.Contains(ItemTag);
	if (ItemExists == false)
	{
		GEngine->AddOnScreenDebugMessage(-1,
				5.f,
			FColor::Red,
			FString::Printf(TEXT("Item %s does not exist in the inventory"),*ItemTag.ToString()));
		return;
	}

	// Check if quantity is not zero
	if (InventoryMap[ItemTag] == 0)
	{
		GEngine->AddOnScreenDebugMessage(-1,
						5.f,
					FColor::Red,
					FString::Printf(TEXT("Items %s Quantity is 0"),*ItemTag.ToString()));
		return;
	}
	
	/*const FName RowName = FName(*ItemTag.ToString());
	FItemBase* FoundItem = ItemDataTable->FindRow<FItemBase>(RowName,TEXT("Looking for row in Data Table"));
	if (FoundItem)
	{
		if (AGASPlayerController* Controller = Cast<AGASPlayerController>(GetOwner()))
		{
			if (Controller->GetAbilitySystemComponent())
			{
				if (UAbilitySystemComponent* AbilitySystemComponent = Controller->GetAbilitySystemComponent())
				{
					FGameplayEffectContextHandle Handle = AbilitySystemComponent->MakeEffectContext();
					FGameplayEffectSpecHandle EffectSpec = AbilitySystemComponent->MakeOutgoingSpec(FoundItem->GameplayEffect,1,Handle);
					AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*EffectSpec.Data.Get());

					AddItem(ItemTag,-1);
				}
			}
		}
	}*/

	const FName RowName = FName(*ItemTag.ToString());
	const FItemBase* FoundItem = ItemDataTable->FindRow<FItemBase>(RowName, TEXT("Looking for row in Data Table"));
	if (!FoundItem || !FoundItem->GameplayEffect)
	{
		UE_LOG(LogTemp, Warning, TEXT("Item or GameplayEffect not found for tag %s"), *ItemTag.ToString());
		return;
	}

	if (IAbilitySystemInterface* AbilitySystemOwner = Cast<IAbilitySystemInterface>(Owner))
	{
		if (UAbilitySystemComponent* AbilitySystemComponent = AbilitySystemOwner->GetAbilitySystemComponent())
		{
			FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
			FGameplayEffectSpecHandle EffectSpec = AbilitySystemComponent->MakeOutgoingSpec(FoundItem->GameplayEffect, 1.0f, EffectContext);
			AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*EffectSpec.Data.Get());
			AddItem(ItemTag, -1);
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1,
			5.f,
			FColor::Blue,
			FString::Printf(TEXT("AbilitySystem not valid")));
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1,
			5.f,
			FColor::Blue,
			FString::Printf(TEXT("AbilitySystemOwner not valid")));	
	}
}

void UInventoryComponent::Server_UseItem_Implementation(FGameplayTag ItemTag)
{
	UseItem(ItemTag);
	GEngine->AddOnScreenDebugMessage(-1,
		5.f,
		FColor::Blue,
		FString::Printf(TEXT("Server_IseItem: Client called server use item")));
}