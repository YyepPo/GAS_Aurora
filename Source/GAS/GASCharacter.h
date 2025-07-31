#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent/GASAbilitySystemComponent.h"
#include "AttributeSet/GASAttributeSet.h"
#include "GASCharacter.generated.h"

/**
 * FLyraReplicatedAcceleration: Compressed representation of acceleration
 */
USTRUCT()
struct FGASReplicatedAcceleration
{
	GENERATED_BODY()

	UPROPERTY()
	uint8 AccelXYRadians = 0;	// Direction of XY accel component, quantized to represent [0, 2*pi]

	UPROPERTY()
	uint8 AccelXYMagnitude = 0;	//Accel rate of XY component, quantized to represent [0, MaxAcceleration]

	UPROPERTY()
	int8 AccelZ = 0;	// Raw Z accel rate component, quantized to represent [-MaxAcceleration, MaxAcceleration]
};

UCLASS(config=Game)
class AGASCharacter : public ACharacter,public  IAbilitySystemInterface
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

public:
	
	AGASCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//~Actor Interface
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;
	virtual void PreReplication(IRepChangedPropertyTracker& ChangedPropertyTracker) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	//~End Of Actor Interface

	
	/** Returns AbilitySystemComponent,this function is part of the  IAbilitySystemInterface **/
    virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override
    {
    	return GameplayAbilitySystemComponent;
    }

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);
	
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	virtual void BeginPlay();

private:

	//~GAS Related Properties
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "GAS",meta = (AllowPrivateAccess = "true"))
		TObjectPtr<UGASAbilitySystemComponent> GameplayAbilitySystemComponent;
	
	UPROPERTY(BlueprintReadOnly,Category = "GAS",meta = (AllowPrivateAccess = "true"))
		TObjectPtr<UGASAttributeSet> GASAttributeSet;

	UPROPERTY(EditAnywhere,Category = "Abilities|Default")
		TArray<TSubclassOf<UGameplayAbility>> DefaultGameplayAbilityClasses;
	UPROPERTY(EditAnywhere,Category = "Abilities|Default")
		TArray<TSubclassOf<UGameplayAbility>> DefaultPassiveGameplayAbilityClasses;
	UPROPERTY(EditAnywhere,Category = "Abilities|Default")
		TSubclassOf<UGameplayEffect> DefaultGameplayEffectClass ;
	
	void InitAbilityInfo();
	//~End GAS Related Properties

	UPROPERTY(Transient,ReplicatedUsing = OnRep_ReplicatedAcceleration)
		FGASReplicatedAcceleration ReplicatedAcceleration;
	UFUNCTION()
		void OnRep_ReplicatedAcceleration();
	
	UPROPERTY(BlueprintReadWrite,meta = (AllowPrivateAccess = "true"))
		AActor* LockedTarget;
	
};

