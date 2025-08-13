#include "GASCharacterBase.h"

AGASCharacterBase::AGASCharacterBase(const FObjectInitializer& ObjectInitializer)
{
		PrimaryActorTick.bCanEverTick = true;

}

void AGASCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AGASCharacterBase::InitAbilityInfo()
{
}

void AGASCharacterBase::BindToAttributeCallbacks()
{
}

