#include "GASCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "AbilitySystemComponent.h"
#include "GASPlayerState.h"
#include "Character/GASMovementComponent.h"
#include "DataAssets/CharacterInfoDataAsset.h"
#include "Net/UnrealNetwork.h"
#include "Other/GASBlueprintFunctionLibrary.h"

struct FCharacterInfo;
class UCharacterInfoDataAsset;

AGASCharacter::AGASCharacter(const FObjectInitializer& ObjectInitializer):
 Super(ObjectInitializer.SetDefaultSubobjectClass<UGASMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	UGASMovementComponent* GASMovementComponent = CastChecked<UGASMovementComponent>(GetCharacterMovement());
	
	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
	
	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void AGASCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if(HasAuthority())
	{
		InitAbilityInfo();
	}
}

void AGASCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	
	// Init ability info on client
	InitAbilityInfo();
}

void AGASCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION(AGASCharacter,ReplicatedAcceleration,COND_SimulatedOnly);
}

void AGASCharacter::PreReplication(IRepChangedPropertyTracker& ChangedPropertyTracker)
{
	Super::PreReplication(ChangedPropertyTracker);

	if (UCharacterMovementComponent* MovementComponent = GetCharacterMovement())
	{
		// Compress Acceleration: XY components as direction + magnitude, Z component as direct value
		const double MaxAccel = MovementComponent->MaxAcceleration;
		const FVector CurrentAccel = MovementComponent->GetCurrentAcceleration();
		double AccelXYRadians, AccelXYMagnitude;
		FMath::CartesianToPolar(CurrentAccel.X, CurrentAccel.Y, AccelXYMagnitude, AccelXYRadians);

		ReplicatedAcceleration.AccelXYRadians   = FMath::FloorToInt((AccelXYRadians / TWO_PI) * 255.0);     // [0, 2PI] -> [0, 255]
		ReplicatedAcceleration.AccelXYMagnitude = FMath::FloorToInt((AccelXYMagnitude / MaxAccel) * 255.0);	// [0, MaxAccel] -> [0, 255]
		ReplicatedAcceleration.AccelZ           = FMath::FloorToInt((CurrentAccel.Z / MaxAccel) * 127.0);   // [-MaxAccel, MaxAccel] -> [-127, 127]
	}
}

void AGASCharacter::OnRep_ReplicatedAcceleration()
{
	if (UGASMovementComponent* GASMovementComponent = Cast<UGASMovementComponent>(GetCharacterMovement()))
	{
		// Decompress Acceleration
		const double MaxAccel         = GASMovementComponent->MaxAcceleration;
		const double AccelXYMagnitude = double(ReplicatedAcceleration.AccelXYMagnitude) * MaxAccel / 255.0; // [0, 255] -> [0, MaxAccel]
		const double AccelXYRadians   = double(ReplicatedAcceleration.AccelXYRadians) * TWO_PI / 255.0;     // [0, 255] -> [0, 2PI]

		FVector UnpackedAcceleration(FVector::ZeroVector);
		FMath::PolarToCartesian(AccelXYMagnitude, AccelXYRadians, UnpackedAcceleration.X, UnpackedAcceleration.Y);
		UnpackedAcceleration.Z = double(ReplicatedAcceleration.AccelZ) * MaxAccel / 127.0; // [-127, 127] -> [-MaxAccel, MaxAccel]

		GASMovementComponent->SetReplicatedAcceleration(UnpackedAcceleration);
	}
}

void AGASCharacter::InitAbilityInfo()
{
	if(AGASPlayerState* GASPlayerState = GetPlayerState<AGASPlayerState>())
    	{
    		// Init Components from player state
    		GASAbilitySystemComponent = GASPlayerState->GetAbilitySystemComp();
    		GASAttributeSet = GASPlayerState->GetAttributeSet();
			GASHealthAttributeSet = GASPlayerState->GetHealthAttributeSet();
		
    		// Init Ability Info for this character
    		if(IsValid(GASAbilitySystemComponent))
    		{
    			// Info about this function -> Initialized the Abilities' ActorInfo - the structure that holds information about who we are acting on and who controls us
    			GASAbilitySystemComponent->InitAbilityActorInfo(GASPlayerState,this);

    			if(HasAuthority() && IsValid(GASAbilitySystemComponent) && IsValid(GASAttributeSet))
    			{
    				if(UCharacterInfoDataAsset* CharacterInfoDataAsset = UGASBlueprintFunctionLibrary::GetCharacterDataInfoAsset(this))
    				{
    					if(const FCharacterInfo* FoundCharacterInfo = CharacterInfoDataAsset->CharacterInfo.Find(CharacterTag))
    					{
    						GASAbilitySystemComponent->AddCharacterAbilities(FoundCharacterInfo->DefaultAbilityClasses);
    						// Activates the given abilities instantly
    						GASAbilitySystemComponent->AddCharacterAbilitiesAndActivate(FoundCharacterInfo->DefaultAutoActivatedAbilityClasses);
    						GASAbilitySystemComponent->AddDefaultGameplayEffects(FoundCharacterInfo->DefaultGameplayEffectClass);
    					}
    					else
    					{
    						GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Red,FString::Printf(TEXT("No Character Tag Selected For Actor: %s"),*GetNameSafe(this)));
    					}
    				}
    			}
    		}
    	}
	
}

void AGASCharacter::Death_Implementation()
{
	if(HasAuthority())
	{
		bIsDead = true;
		OnRep_IsDead();
	}
	else
	{
		Server_SetIsDead();
	}
}

void AGASCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// Input
void AGASCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AGASCharacter::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AGASCharacter::Look);

		
	}
}

void AGASCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// Get right vector for moving sideways
		AddMovementInput(RightDirection, MovementVector.X);
	
		// Forward/Backward movement
		AddMovementInput(ForwardDirection, MovementVector.Y);
	}
}

void AGASCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}



