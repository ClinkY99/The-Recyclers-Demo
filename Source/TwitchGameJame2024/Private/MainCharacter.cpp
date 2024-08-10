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
#include "Gameplay/Workstation.h"
#include "Kismet/GameplayStatics.h"
#include "InputActionValue.h"
#include "StartInterface.h"
#include "Gameplay/RecyclingInput.h"
#include "Buildings/CubeStorage.h"
#include "Contracts/contracts_base.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

// Sets default values
AMainCharacter::AMainCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(0.f, 0.f);

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);

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

	currentRoundTime = defaultRoundTime;
	numTrucks = defaultNumTrucks;

	WorkerPlaceMarker = GetWorld()->SpawnActor<AActor>(workerPlaceMarkerClass);
	WorkerPlaceMarker->SetActorLocation(FVector(0, 0, -500));

	playPreRoundMusic();
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
				buildingHolo->SetActorLocation(snapToGrid2d(Hit.Location, gridSize, 0));
				if (money < priceOfCurrentBuilding) {
					buildingHolo->makeInvalid();
				}
			}
		}
	}
	else if (inBuildModeWorker) {
		if (APlayerController* PlayerController = Cast<APlayerController>(Controller)) {
			FHitResult Hit;
			FVector location;
			PlayerController->GetHitResultUnderCursor(ECC_WorldDynamic, true, Hit);

			if (Hit.GetActor()) {
				location.X = Hit.Location.X;
				location.Y = Hit.Location.Y;
				location.Z = 0;

				WorkerPlaceMarker->SetActorLocation(location);
			}
		}
	}
	else if (inEraseMode) {
		if (APlayerController* PlayerController = Cast<APlayerController>(Controller)) {
			FHitResult Hit;
			FVector location;
			PlayerController->GetHitResultUnderCursor(ECC_WorldDynamic, true, Hit);
			if (Hit.GetActor()) {
				if (ABuildablesBase* building = Cast<ABuildablesBase>(Hit.GetActor())) {
					if (hoveredBuilding != building && hoveredBuilding) {
						hoveredBuilding->removeHolo();
					}
					hoveredBuilding = building;
					if (placedThisTurn.Contains(hoveredBuilding)) {
						building->makeInvalid();
					}
					if (deleteBox) {
						deleteBox->Destroy();
						deleteBox = nullptr;
					}
				}
				else {
					if (hoveredBuilding) {
						hoveredBuilding->removeHolo();
					}
					if (!deleteBox) {
						deleteBox = GetWorld()->SpawnActor<AActor>(deleteBoxClass);
					}

					deleteBox->SetActorLocation(snapToGrid2d(Hit.Location, gridSize, deleteBox->GetSimpleCollisionHalfHeight()));
				}
			}
		}
		
	} 
	else {
		if (APlayerController* PlayerController = Cast<APlayerController>(Controller)) {
			FHitResult Hit;

			PlayerController->GetHitResultUnderCursor(ECC_WorldDynamic, true, Hit);
			
			if (IInteractInterface* actorRef = Cast<IInteractInterface>(Hit.GetActor())) {
				if (HoveredActor!=actorRef && HoveredActor) {
					HoveredActor->hover(false);
				}
				actorRef->hover(true);
				HoveredActor = actorRef;
			}
			else if (HoveredActor) {
				HoveredActor->hover(false);
				HoveredActor = nullptr;
			}
			
		}


	}
}

void AMainCharacter::enterBuildMode(FBuildableStruct buildingInfo)
{
	exitMode();
	selectedBuildingInfo = buildingInfo;
	priceOfCurrentBuilding = buildingInfo.cost;
	buildingRotation = FRotator();

	spawnNewPlaceable();

	inBuildMode = true;
}

void AMainCharacter::enterBuildModeWorker(int32 price)
{
	exitMode();
	priceOfCurrentBuilding = price;
	inBuildModeWorker = true;
}

void AMainCharacter::spawnDropOffTruckTruck() {
	AActor* truck = GetWorld()->SpawnActor<AActor>(TruckActor);
	truck->SetActorLocation(FVector(0, 0, -500));
}

void AMainCharacter::StartRound()
{
	moneyMadeInRound = 0;
	if (!isInRound) {
		if (roundNum % incrementToIncreaseRoundTime == 0) {
			currentRoundTime += increaseTime;
		}

		TArray<AActor*> actors;
		UGameplayStatics::GetAllActorsWithInterface(GetWorld(), UStartInterface::StaticClass(), actors);
		for (auto& actor : actors) {
			if (IStartInterface* actorCast = Cast<IStartInterface>(actor)) {
				actorCast->startRound();

			}
		}
		isInRound = true;
		exitMode();

		GetWorldTimerManager().SetTimer(roundTimer, this, &AMainCharacter::EndRound, currentRoundTime, false);

		for (size_t i = 0; i < morningRush; i++)
		{
			spawnDropOffTruckTruck();
		}

		GetWorldTimerManager().SetTimer(truckTimer, this, &AMainCharacter::spawnDropOffTruckTruck, currentRoundTime/numTrucks, true);

		FTimerHandle midDayRushTimer;

		GetWorldTimerManager().SetTimer(midDayRushTimer, this, &AMainCharacter::middayRush, currentRoundTime / 2, false);

		playRoundMusic();
	}
}

void AMainCharacter::middayRush()
{
	for (size_t i = 0; i < midDayRush; i++)
	{
		spawnDropOffTruckTruck();
	}
}

void AMainCharacter::EndRound()
{
	if (isInRound) {
		TArray<AActor*> actors;
		UGameplayStatics::GetAllActorsWithInterface(GetWorld(), UStartInterface::StaticClass(), actors);
		for (auto& actor : actors) {
			if (IStartInterface* actorCast = Cast<IStartInterface>(actor)) {
				actorCast->endRound();

			}
		}
		GetWorldTimerManager().ClearTimer(truckTimer);

		TArray<AActor*> actor2;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACubeStorage::StaticClass(), actor2);

		for (auto& actor : actor2) {
			if (ACubeStorage* actorCast = Cast<ACubeStorage>(actor)) {
				moneyMadeInRound += actorCast->cubes.Num() * moneyPerCube * (100*moneyBonus +1);
				actorCast->removeCubes();
			}
		}
		if (moneyMadeInRound < 1) {
			moneyMadeInRound += 30;
		}

		money += moneyMadeInRound;

		isInRound = false;
		exitMode();

		if (roundNum % incrementToGiveAddTrucks == 0) {
			numTrucks += FMath::RandRange(1, increaseAmountMax);
		}
		if (roundNum % incrementToIncreaseOverflow == 0) {
			AActor* actorRef = UGameplayStatics::GetActorOfClass(GetWorld(), ARecyclingInput::StaticClass());
			if (ARecyclingInput* recyclingInput = Cast<ARecyclingInput>(actorRef)) {
				recyclingInput->maxRecyclingBeforeOverflow += increaseOverflowBy;
			}
		}

		showEndScreen(roundNum % incrementToGiveContract == 0);
	}
}

void AMainCharacter::contractPicked(FContractStruct contract) {
	AActor* contractref = UGameplayStatics::GetActorOfClass(GetWorld(), contract.classRef);

	if (AContracts_base* contractActor = Cast<AContracts_base>(contractref)) {
		contractActor->picked();
		moneyBonus += contract.moneyBonus;
	}	
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
		EnhancedInputComponent->BindAction(pressedLClickAction, ETriggerEvent::Triggered, this, &AMainCharacter::pressedLClick);
		EnhancedInputComponent->BindAction(RClickAction, ETriggerEvent::Triggered, this, &AMainCharacter::RClick);
		EnhancedInputComponent->BindAction(EscapeAction, ETriggerEvent::Triggered, this, &AMainCharacter::escapePressed);
		EnhancedInputComponent->BindAction(EraseAction, ETriggerEvent::Triggered, this, &AMainCharacter::erasePressed);
		EnhancedInputComponent->BindAction(StartRoundAction, ETriggerEvent::Triggered, this, &AMainCharacter::startRoundPressed);
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

}

void AMainCharacter::pressedLClick(const FInputActionValue& Value)
{
	if (inBuildMode) {
		if (buildingHolo) {
			if (!buildingHolo->isInvalid) {
				money -= priceOfCurrentBuilding;
				buildingHolo->removeHolo();
				buildingHolo->build();
				placedThisTurn.Add(buildingHolo);

				spawnNewPlaceable();
			}
		}
	}
	else if (inEraseMode) {
		if (hoveredBuilding) {
			if (placedThisTurn.Contains(hoveredBuilding)) {
				destroyActor();
				money += 40;
			}
		}
	}
	else if (inBuildModeWorker) {
		TArray<AActor*> workers;
		TArray<AActor*> workstations;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AWorker::StaticClass(), workers);
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AWorkstation::StaticClass(), workstations);

		if (workers.Num() < workstations.Num() && priceOfCurrentBuilding <= money) {
			money -= priceOfCurrentBuilding;
			AWorker* worker = GetWorld()->SpawnActor<AWorker>(workerClass);
			if (worker) {
				worker->SetActorLocation(WorkerPlaceMarker->GetActorLocation());
			}
		}

	}
	else if (SelectedActor == HoveredActor && SelectedActor) {
		SelectedActor->unselect(this);
		SelectedActor = nullptr;
	}
	else if (inPathingMode && HoveredActor) {
		if (AWorkstation* actorRef = Cast<AWorkstation>(HoveredActor)) {
			if (!actorRef->GetWorker()) {
				AWorker* workerRef = Cast<AWorker>(SelectedActor);
				workerRef->setNewWorkStation(actorRef);
				actorRef->SetWorker(workerRef);
				actorRef->select(this);
			}
		}
		else {
			SelectedActor->unselect(this);
			SelectedActor = HoveredActor;
			SelectedActor->select(this);
		}
	}
	
	else if (SelectedActor && HoveredActor) {
		SelectedActor->unselect(this);
		SelectedActor = HoveredActor;
		SelectedActor->select(this);
	}
	else if (HoveredActor) {
		HoveredActor->select(this);
		SelectedActor = HoveredActor;
	}
}

void AMainCharacter::RClick(const FInputActionValue& Value)
{
	if (inBuildMode) {
		buildingRotation.Yaw += 90;
		if (buildingRotation.Yaw == 360) {
			buildingRotation.Yaw = 0;
		}
		buildingHolo->SetActorRotation(buildingRotation);
	}
}

void AMainCharacter::escapePressed(const FInputActionValue& Value)
{
	if (inBuildMode || inEraseMode || inBuildModeWorker) {
		exitMode();
	}
	else if (SelectedActor) {
		SelectedActor->unselect(this);
		SelectedActor = nullptr;
	}
	else {
		pauseGame();
	}
}

void AMainCharacter::erasePressed(const FInputActionValue& Value)
{
	if (!inEraseMode && !isInRound) {
		if (inBuildMode) {
			inBuildMode = false;
			buildingHolo->Destroy();
		}
		inEraseMode = true;
		deleteBox = GetWorld()->SpawnActor<AActor>(deleteBoxClass);
	}
}

void AMainCharacter::startRoundPressed()
{
	if (!isInRound) {
		if (GetWorldTimerManager().IsTimerActive(startRoundTimer)) {
			GetWorldTimerManager().ClearTimer(startRoundTimer);
		}
		else {
			GetWorldTimerManager().SetTimer(startRoundTimer, this, &AMainCharacter::StartRound, timeToCancel, false);
		}
	}
}

void AMainCharacter::destroyActor()
{
	placedThisTurn.Remove(hoveredBuilding);
	hoveredBuilding->destroyBuilding();
}

void AMainCharacter::exitMode()
{
	if (inBuildMode) {
		inBuildMode = false;

		selectedBuildingInfo = FBuildableStruct();
		buildingHolo->Destroy();
	}
	else if (inBuildModeWorker) {
		inBuildModeWorker = false;
		WorkerPlaceMarker->SetActorLocation(FVector(0, 0, -500));
	}
	else if (inEraseMode){
		if (deleteBox) {
			deleteBox->Destroy();
		}
		inEraseMode = false;
	}
}
