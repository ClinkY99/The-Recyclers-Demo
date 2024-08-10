// Fill out your copyright notice in the Description page of Project Settings.


#include "Worker.h"
#include "Components/CapsuleComponent.h"
#include "MainCharacter.h"
#include "AIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AWorker::AWorker(){
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	GetMesh()->SetCustomDepthStencilValue(0);

 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AWorker::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWorker::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (inRound && !workstation) {
		GetMesh()->SetCustomDepthStencilValue(1);
		GetMesh()->SetRenderCustomDepth(true);

	}

	ACharacter* character = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	if (AMainCharacter* mainCharacter = Cast<AMainCharacter>(character)) {
		workEfficiency = mainCharacter->workerEfficiency;
	}
}

// Called to bind functionality to input
void AWorker::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AWorker::hover(bool isHovered)
{
	if (!isSelected && !(inRound && !workstation)) {
		GetMesh()->SetRenderCustomDepth(0);
		GetMesh()->SetRenderCustomDepth(isHovered);
	}
}


void AWorker::select(AMainCharacter* character)
{
	isSelected = true;

	character->inPathingMode = true;

	if (workstation) {
		workstation->select(character);
	}
}


void AWorker::unselect(AMainCharacter* character)
{
	GetMesh()->SetRenderCustomDepth(false);
		
	character->inPathingMode = false;

	isSelected = false;

	if (workstation) {
		workstation->unselect(character);
	}
}

void AWorker::startRound()
{
	inRound = true;
}
void AWorker::endRound()
{
	inRound = false;
}

void AWorker::updateWorkBehavior(UBehaviorTree* NewBehavior)
{
	if (AAIController* AIcontroller = Cast<AAIController>(Controller)) {
		AIcontroller->BrainComponent->StopLogic("newWorkDirective");
		AIcontroller->RunBehaviorTree(NewBehavior);
	}
}

