// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "GASInputConfig.h"
#include "GASEnhancedInputComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GAS_API UGASEnhancedInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

public:
	
	template<class UserClass,typename PressedFuncType,typename ReleasedFuncType>
		void BindAbilityActions(UGASInputConfig* Input,UserClass* Object,PressedFuncType PressedFunc,ReleasedFuncType ReleasedFunc);
	
};

template <class UserClass, typename PressedFuncType, typename ReleasedFuncType>
void UGASEnhancedInputComponent::BindAbilityActions(UGASInputConfig* Input, UserClass* Object,
	PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc)
{
	check(Input);

	for (const FGASInputAction& InputAction : Input->InputActions)
	{
		if(IsValid(InputAction.Input) && InputAction.InputTag.IsValid())
		{
			if(PressedFunc)
			{
				BindAction(InputAction.Input,ETriggerEvent::Started,Object,PressedFunc,InputAction.InputTag);
			}

			if(ReleasedFunc)
			{
				BindAction(InputAction.Input,ETriggerEvent::Completed,Object,ReleasedFunc,InputAction.InputTag);
			}
		}
	}
}
