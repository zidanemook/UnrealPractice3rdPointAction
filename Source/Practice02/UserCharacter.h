// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "./Define.h"

#include "GameFramework/Character.h"
#include "Weapon.h"
#include "Pistol.h"
#include "Shotgun.h"
#include "RocketLauncher.h"


#include "UserCharacter.generated.h"

UCLASS()
class PRACTICE02_API AUserCharacter : public ACharacter
{
	GENERATED_UCLASS_BODY()

public:
	// Sets default values for this character's properties
	//AUserCharacter();

protected:
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FollowCamera;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseLookUpRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AniVariable, meta = (AllowPrivateAccess = "true"))
		bool	JogPressed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AniVariable, meta = (AllowPrivateAccess = "true"))
		bool	CrouchPressed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AniVariable, meta = (AllowPrivateAccess = "true"))
		bool	PronePressed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AniVariable, meta = (AllowPrivateAccess = "true"))
		bool	DisableMovement;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AniVariable, meta = (AllowPrivateAccess = "true"))
		bool	JumpPressed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AniVariable, meta = (AllowPrivateAccess = "true"))
		bool	IsAiming;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AniVariable, meta = (AllowPrivateAccess = "true"))
		bool	DoAttack;//Slash, firing...

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AniVariable, meta = (AllowPrivateAccess = "true"))
		bool	IsGun;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AniVariable, meta = (AllowPrivateAccess = "true"))
		bool	IsBow;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AniVariable, meta = (AllowPrivateAccess = "true"))
		Equipment_Type eEquipmentType;


	/* Collision for character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collision)
		UBoxComponent* CollisionComp;

	UPROPERTY(EditDefaultsOnly, Category = DefaultInv)
		TSubclassOf<class AWeapon> WeaponSpawn;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Weapon)
		AWeapon *CurrentWeapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Inventory)
		TArray<class AWeapon*> Inventory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Widgets)
		TSubclassOf<UUserWidget> wHUD;

	// Variable to hold the widget After Creating it.
	UUserWidget* m_HUD;

protected:
	// Called when the game starts or when spawned
	UFUNCTION(BlueprintCallable, Category = Event)
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/**
	* Called via input to turn at a given rate.
	* @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	*/
	void TurnAtRate(float Rate);

	/**
	* Called via input to turn look up/down at a given rate.
	* @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	*/
	void LookUpAtRate(float Rate);

	void Interact();

	void Jog();

	void StopJog();

	void Crouching();

	void Prone();

	void Jump();

	void StopJumping();

	void SwitchTool();

	void LeftMouseButtonPressed();
	void LeftMouseButtonReleased();

	void RightMouseButtonPressed();
	void RightMouseButtonReleased();

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	//void NumKeyZero();
	//void NumKeyOne();
	//void NumKeyTwo();
	//void NumKeyThree();
	//void NumKeyFour();
	//void NumKeyFive();
	//void NumKeySix();
	//void NumKeySeven();
	//void NumKeyEight();
	//void NumKeyNine();



	UFUNCTION()
		void OnCollision(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

	void ProcessWeaponPickup(AWeapon *Weapon);
	void EquipWeapon(AWeapon *Weapon);
	void GiveDefaultWeapon();
};
