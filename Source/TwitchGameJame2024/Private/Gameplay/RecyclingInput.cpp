// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/RecyclingInput.h"

// Sets default values
ARecyclingInput::ARecyclingInput()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	mesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("Mesh"));
	mesh->SetupAttachment(RootComponent);

	mesh->SetCustomDepthStencilValue(0);

}

// Called when the game starts or when spawned
void ARecyclingInput::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARecyclingInput::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARecyclingInput::hover(bool isHovered)
{
	if (!isSelected) {
		mesh->SetRenderCustomDepth(isHovered);
	}
}

void ARecyclingInput::select(AMainCharacter* character)
{
	mesh->SetRenderCustomDepth(true);
	isSelected = true;
}

void ARecyclingInput::unselect(AMainCharacter* character)
{
	mesh->SetRenderCustomDepth(false);
	isSelected = false;
}

void ARecyclingInput::startRound()
{
}

void ARecyclingInput::endRound()
{
}

void ARecyclingInput::upgrade()
{
}

void ARecyclingInput::addResource()
{
}


