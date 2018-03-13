// Fill out your copyright notice in the Description page of Project Settings.

#include "UserCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"


// Sets default values
AUserCharacter::AUserCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 400.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

												// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

	GetCharacterMovement()->MaxWalkSpeed = 200;

	// Init AniVariables
	JogPressed = false;
	CrouchPressed = false;
	PronePressed = false;
	DisableMovement = false;
	JumpPressed = false;
	EquipTool = false;
	bUseControllerRotationYaw = false;
	eEquipmentType = Equipment_Type::Equip_None;
}

// Called when the game starts or when spawned
void AUserCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AUserCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AUserCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Set up gameplay key bindings
	check(PlayerInputComponent);

	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AUserCharacter::Interact);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AUserCharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AUserCharacter::StopJumping);

	PlayerInputComponent->BindAction("Jog", IE_Pressed, this, &AUserCharacter::Jog);
	PlayerInputComponent->BindAction("Jog", IE_Released, this, &AUserCharacter::StopJog);

	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &AUserCharacter::Crouching);

	PlayerInputComponent->BindAction("SwitchTool", IE_Pressed, this, &AUserCharacter::SwitchTool);

	PlayerInputComponent->BindAction("Prone", IE_Pressed, this, &AUserCharacter::Prone);

	PlayerInputComponent->BindAction("LeftMouseButton", IE_Pressed, this, &AUserCharacter::LeftMouseButtonPressed);
	PlayerInputComponent->BindAction("LeftMouseButton", IE_Released, this, &AUserCharacter::LeftMouseButtonReleased);

	//PlayerInputComponent->BindAction("0", IE_Pressed, this, &AUserCharacter::NumKeyZero);
	//PlayerInputComponent->BindAction("1", IE_Pressed, this, &AUserCharacter::NumKeyOne);
	//PlayerInputComponent->BindAction("2", IE_Pressed, this, &AUserCharacter::NumKeyTwo);
	//PlayerInputComponent->BindAction("3", IE_Pressed, this, &AUserCharacter::NumKeyThree);
	//PlayerInputComponent->BindAction("4", IE_Pressed, this, &AUserCharacter::NumKeyFour);
	//PlayerInputComponent->BindAction("5", IE_Pressed, this, &AUserCharacter::NumKeyFive);
	//PlayerInputComponent->BindAction("6", IE_Pressed, this, &AUserCharacter::NumKeySix);
	//PlayerInputComponent->BindAction("7", IE_Pressed, this, &AUserCharacter::NumKeySeven);
	//PlayerInputComponent->BindAction("8", IE_Pressed, this, &AUserCharacter::NumKeyEight);
	//PlayerInputComponent->BindAction("9", IE_Pressed, this, &AUserCharacter::NumKeyNine);

	PlayerInputComponent->BindAxis("MoveForward", this, &AUserCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AUserCharacter::MoveRight);
	
	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	//PlayerInputComponent->BindAxis("TurnRate", this, &AUserCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	//PlayerInputComponent->BindAxis("LookUpRate", this, &AUserCharacter::LookUpAtRate);
}

//void AUserCharacter::TurnAtRate(float Rate)
//{
//	// calculate delta for this frame from the rate information
//	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
//}
//
//void AUserCharacter::LookUpAtRate(float Rate)
//{
//	// calculate delta for this frame from the rate information
//	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
//}

void AUserCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f) && (false == PronePressed) && (false == DisableMovement))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		if (EquipTool && (Value < 0))
			Value *= 0.7f;
		AddMovementInput(Direction, Value);
	}
}

void AUserCharacter::MoveRight(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f) && (false == PronePressed) && (false == DisableMovement))
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void AUserCharacter::Interact()
{
}

void AUserCharacter::Jog()
{
	JogPressed = true;
	GetCharacterMovement()->MaxWalkSpeed = 375;
}

void AUserCharacter::StopJog()
{
	JogPressed = false;

	if (true == CrouchPressed)
	{
		GetCharacterMovement()->MaxWalkSpeed = 160;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = 200;
	}
}

void AUserCharacter::Crouching()
{
	ACharacter::Crouch();
	CrouchPressed = !CrouchPressed;

	if (true == CrouchPressed)
	{
		GetCharacterMovement()->MaxWalkSpeed = 160;
	}
	else
	{
		if (true == JogPressed)
		{
			GetCharacterMovement()->MaxWalkSpeed = 375;
		}
		else
		{
			GetCharacterMovement()->MaxWalkSpeed = 200;
		}
	}
}

void AUserCharacter::Prone()
{
	PronePressed = !PronePressed;
}

void AUserCharacter::Jump()
{
	ACharacter::Jump();

	if ((false == CrouchPressed) && (false == DisableMovement))
	{
		JumpPressed = true;
	}
}
void AUserCharacter::StopJumping()
{
	ACharacter::StopJumping();

	JumpPressed = false;
}

void AUserCharacter::SwitchTool()
{
	uint8 iType = (uint8)eEquipmentType;
	iType += 1;
	eEquipmentType = (Equipment_Type)iType;
	if (iType >= (uint8)Equipment_Type::Equip_Max)
		eEquipmentType = Equipment_Type::Equip_None;

	if (eEquipmentType == Equipment_Type::Equip_None)
	{
		EquipTool = false;
		bUseControllerRotationYaw = false;
	}
	else
	{
		bUseControllerRotationYaw = true;
		EquipTool = true;	
	}

	const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("Equipment_Type"), true);
	if (EnumPtr)
	{
		FString text = (EnumPtr->GetNameByValue((int64)eEquipmentType)).ToString();
		UE_LOG(LogTemp, Warning, TEXT("EquipTool is %s"), *text);
	}
}

void AUserCharacter::LeftMouseButtonPressed()
{
	DisableMovement = true;
}

void AUserCharacter::LeftMouseButtonReleased()
{
	DisableMovement = false;
}

//총이냐 근접무기냐 활이냐 정보를 애니메이션에 전달 어떻게?