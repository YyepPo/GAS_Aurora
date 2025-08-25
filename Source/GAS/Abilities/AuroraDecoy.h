#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GAS/AbilitySystemComponent/GASAbilitySystemComponent.h"
#include "GameFramework/Actor.h"
#include "GAS/AttributeSet/GASHealthAttributeSet.h"
#include "AuroraDecoy.generated.h"

class UGASAbilitySystemComponent;

UCLASS()
class GAS_API AAuroraDecoy : public AActor,public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	
	AAuroraDecoy();

	UFUNCTION(BlueprintImplementableEvent)
		void OnHealthChanged(float CurrentValue,float MaxHealth);
	
protected:
	
	virtual void BeginPlay() override;

	/** Returns AbilitySystemComponent,this function is part of the  IAbilitySystemInterface **/
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override
	{
		return GASAbilitySystemComponent;
	}

	UFUNCTION(BlueprintCallable)
		void InitGameplayEffect();
	UFUNCTION(BlueprintCallable)
		void BindToAttributeCallbacks();
	
private:

	UPROPERTY(BlueprintReadWrite,meta = (AllowPrivateAccess = true))
		TObjectPtr<UGASAbilitySystemComponent> GASAbilitySystemComponent;
	UPROPERTY(BlueprintReadWrite,meta = (AllowPrivateAccess = true))
		TObjectPtr<UGASHealthAttributeSet> GASAttributeSet;
	UPROPERTY(EditDefaultsOnly)
		FGameplayTag CharacterTag;	
};
