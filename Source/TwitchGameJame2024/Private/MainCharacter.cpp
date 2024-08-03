// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Buildings/BuildablesBase.h"
#include "InputActionValue.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

// Sets default values
AMainCharacter::AMainCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(0.f, 0.f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 5000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

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

	PrimaryActorTick.bCanEverTick = true;

	inBuildMode = false;
}

void AMainCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(true);
}

void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (inBuildMode) {
		if (APlayerController* PlayerController = Cast<APlayerController>(Controller)) {
			FHitResult Hit;
			FVector location;
			PlayerController->GetHitResultUnderCursor(ECC_WorldDynamic, true, Hit);

			if (Hit.GetActor()) {
				buildingHolo->SetActorLocation(snapToGrid2d(Hit.Location, 100, buildingHolo->GetSimpleCollisionHalfHeight()));
			}
		}
	}
}

void AMainCharacter::enterBuildMode(FBuildableStruct buildingInfo)
{
	selectedBuildingInfo = buildingInfo;
	buildingRotation = FRotator();

	spawnNewPlaceable();

	inBuildMode = true;
}

void AMainCharacter::spawnNewPlaceable()
{
	buildingHolo = GetWorld()->SpawnActor<ABuildablesBase>(selectedBuildingInfo.classRef);
	buildingHolo->makeHolo();
	buildingHolo->SetActorRotation(buildingRotation);
}

//////////////////////////////////////////////////////////////////////////
// Input

void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMainCharacter::Move);
		EnhancedInputComponent->BindAction(LClickAction, ETriggerEvent::Triggered, this, &AMainCharacter::LClick);
		EnhancedInputComponent->BindAction(RClickAction, ETriggerEvent::Triggered, this, &AMainCharacter::RClick);
		EnhancedInputComponent->BindAction(EscapeAction, ETriggerEvent::Triggered, this, &AMainCharacter::escapePressed);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AMainCharacter::Move(const FInputActionValue& Value)
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

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AMainCharacter::LClick(const FInputActionValue& Value) {
	if (inBuildMode){
		if (!buildingHolo->isOverlap) {
			buildingHolo->removeHolo();

			spawnNewPlaceable();
		}
	}
	else {
		//line cast and check if it is clicking a building
	}
}

void AMainCharacter::RClick(const FInputActionValue& Value)
{
	if (inBuildMode) {
		buildingRotation.Yaw += 90;

		buildingHolo->SetActorRotation(buildingRotation);
	}
}

void AMainCharacter::escapePressed(const FInputActionValue& Value)
{
	if (inBuildMode) {
		inBuildMode = false;

		selectedBuildingInfo = FBuildableStruct();
		buildingHolo->Destroy();
		buildingHolo = nullptr;

	}
	else {
		//pause game
	}
}

