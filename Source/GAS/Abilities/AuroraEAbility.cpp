#include "GAS/Abilities/AuroraEAbility.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GAS/AbilitySystemComponent/GASAbilitySystemComponent.h"
#include "GAS/DataAssets/IceSpikeDataAsset.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Task/PlayMontageAndWaitWithAnimNotify.h"

UAuroraEAbility::UAuroraEAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UAuroraEAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                      const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                      const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (IsIceSpikeDataAssetValid() == false) 
	{
		EndAbility(Handle,ActorInfo,ActivationInfo,false,true);
		return;
	}
	
	const bool bCanActivateAbility = CanActivateAbility(Handle,ActorInfo);
	if(bCanActivateAbility == false)
	{
		EndAbility(Handle,ActorInfo,ActivationInfo,false,true);
		return;
	}

	const bool bCanCommitAbility = CommitAbility(Handle,ActorInfo,ActivationInfo);
	if(bCanCommitAbility == false)
	{
		EndAbility(Handle,ActorInfo,ActivationInfo,false,true);
		return;
	}
	
	UPlayMontageAndWaitWithAnimNotify* MontageTask = UPlayMontageAndWaitWithAnimNotify::CreatePlayMontageAndWaitWithNotifyProxy(this,NAME_None,IceSpikeDataAsset->IceSpikeProperties.MontageToPlay);
	if(MontageTask)
	{
		EnableMovement(false);
		
		MontageTask->OnAnimNotifyTriggered.AddDynamic(this,&UAuroraEAbility::OnMontageAnimNotifyTriggered);
		MontageTask->OnMontageEndedDelegate.AddDynamic(this,&UAuroraEAbility::OnMontageCompleted);
		MontageTask->OnMontageBlendedOut.AddDynamic(this,&UAuroraEAbility::OnMontageCompleted);
		MontageTask->ReadyForActivation();
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Red,FString::Printf(TEXT("No montage task in AuroraEAbility")));
	}
	
}

void UAuroraEAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                 const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if (IsIceSpikeDataAssetValid() == false) 
	{
		return;
	}

	ACharacter* Character = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
	if (Character && IceSpikeDataAsset->IceSpikeProperties.MontageToPlay)
	{
		UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();
		if (AnimInstance && AnimInstance->Montage_IsPlaying(IceSpikeDataAsset->IceSpikeProperties.MontageToPlay))
		{
			AnimInstance->Montage_Stop(0.f,IceSpikeDataAsset->IceSpikeProperties.MontageToPlay);
		}
	}

	EnableMovement(true);

	UE_LOG(LogTemp, Log, TEXT("Ability %s ended. Cancelled: %s"), *GetName(), bWasCancelled ? TEXT("Yes") : TEXT("No"));
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UAuroraEAbility::OnMontageCompleted()
{
	EndAbility(CurrentSpecHandle,CurrentActorInfo,CurrentActivationInfo,false,false);
}

void UAuroraEAbility::OnMontageInterrupted()
{
	EndAbility(CurrentSpecHandle,CurrentActorInfo,CurrentActivationInfo,false,true);
}

void UAuroraEAbility::OnMontageAnimNotifyTriggered(FName AnimNotifyText)
{
	const FVector Center = GetAvatarActorFromActorInfo()->GetActorLocation();

	if(AnimNotifyText == FName("IceSpikes"))
	{
		if (IsIceSpikeDataAssetValid() == false) 
		{
			return;
		}

		// Calculate spawn points for visual effect.
		TArray<FVector_NetQuantize> Locations;
		TArray<FVector_NetQuantize> LookAtRotations;
		TArray<float> EmitterScales;

		for (int32 i = 0; i < IceSpikeDataAsset->IceSpikeProperties.NumberOfSpikes; i++)
		{
			const float Angle = (i / (float)IceSpikeDataAsset->IceSpikeProperties.NumberOfSpikes) * 2.0f * PI;
		
			const float X = Center.X + IceSpikeDataAsset->IceSpikeProperties.SphereRadius * FMath::Cos(Angle);
			const float Y = Center.Y + IceSpikeDataAsset->IceSpikeProperties.SphereRadius * FMath::Sin(Angle);
			
			FVector SpawnLocation = FVector(X, Y, Center.Z);
			// Make sure that the SpawnLocation.Z is on the floor
			FHitResult HitResult;
			GetWorld()->LineTraceSingleByChannel(HitResult,
				SpawnLocation,
				SpawnLocation + SpawnLocation.UpVector * -IceSpikeDataAsset->IceSpikeProperties.GroundTraceDistance,
				ECC_Vehicle);

			SpawnLocation.Z = HitResult.bBlockingHit ? SpawnLocation.Z = HitResult.ImpactPoint.Z : SpawnLocation.Z = HitResult.TraceEnd.Z;
			
			Locations.Emplace(SpawnLocation);
		}

		// Calculate emitter scales and rotations
        float EmitterScale = 0.5f;
        for (int32 i = 0; i < Locations.Num(); i++)
        {
            if (EmitterScale <= 1.0f)
            {
                EmitterScale += IceSpikeDataAsset->IceSpikeProperties.EmitterScaleIncrement;
            }
        	EmitterScales.Emplace(EmitterScale);
            const int32 a = (i + 1) % Locations.Num();
            const FRotator Rotation = UKismetMathLibrary::FindLookAtRotation(Locations[i], Locations[a]);
			LookAtRotations.Emplace(Rotation.Vector());
            if (GetAvatarActorFromActorInfo()->HasAuthority())
            {
                const TArray<AActor*> ActorsToIgnore;
                FHitResult OutHit;
                const bool bHit = UKismetSystemLibrary::SphereTraceSingle(
                    GetWorld(),
                    Locations[i],
                    Locations[i],
                    50.0f,
                    ETraceTypeQuery::TraceTypeQuery1,
                    false,
                    ActorsToIgnore,
                    EDrawDebugTrace::ForDuration,
                    OutHit,
                    true,
                    FLinearColor::Red,
                    FLinearColor::Green,
                    5.0f
                );

                if (bHit)
                {
                    AActor* HitActor = OutHit.GetActor();
                    if (HitActor && HitActor->ActorHasTag("Character"))
                    {
                        if (IAbilitySystemInterface* AbilitySystemInterface = Cast<IAbilitySystemInterface>(HitActor))
                        {
                            if (UGASAbilitySystemComponent* TargetASC = Cast<UGASAbilitySystemComponent>(AbilitySystemInterface->GetAbilitySystemComponent()))
                            {
                                FGameplayTag FreezeAbilityTag = FGameplayTag::RequestGameplayTag(TEXT("Ability.Stunned"));
                                TargetASC->ActivateAbilityByTag(FreezeAbilityTag);
                            }
                        }
                    }
                }
            }
        }
		SpawnIceSparksVFXReplicated(Locations,LookAtRotations,EmitterScales);
	}
}

bool UAuroraEAbility::IsIceSpikeDataAssetValid() const
{
	if(IsValid(IceSpikeDataAsset) == false)
	{
		GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Red,FString::Printf(TEXT("Ice Spike Data asset is not selected in AuroraEAbility. Pls select a data asset")));
		return false;
	}
	return true;
}



