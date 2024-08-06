// Fill out your copyright notice in the Description page of Project Settings.

#include "Worker.h"
#include "Gameplay/Workstation.h"

// Sets default values
AWorkstation::AWorkstation()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	mesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("Mesh"));
	mesh->SetupAttachment(RootComponent);

	mesh->SetCustomDepthStencilValue(0);

}

// Called when the game starts or when spawned
void AWorkstation::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWorkstation::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWorkstation::hover(bool isHovered)
{
	if (!isSelected) {
		mesh->SetRenderCustomDepth(isHovered);
	}
}

void AWorkstation::select(AMainCharacter* character)
{
	mesh->SetRenderCustomDepth(true);
	isSelected = true;
}

void AWorkstation::unselect(AMainCharacter* character)
{
	mesh->SetRenderCustomDepth(false);
	isSelected = false;
}

