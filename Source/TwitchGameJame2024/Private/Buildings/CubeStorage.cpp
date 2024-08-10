// Fill out your copyright notice in the Description page of Project Settings.


#include "Buildings/CubeStorage.h"
#include "Engine/StaticMeshActor.h"
#include "../MainCharacter.h"
#include "Kismet/GameplayStatics.h"

ACubeStorage::ACubeStorage()
{
	FirstSpawnSpot = CreateDefaultSubobject<USceneComponent>(FName("firstCubeSpawnLocation"));
	FirstSpawnSpot->SetupAttachment(mesh);

	dropOffPoint = CreateDefaultSubobject<USceneComponent>(FName("DropOffLocation"));
	dropOffPoint->SetupAttachment(mesh);
}

void ACubeStorage::BeginPlay()
{
	Super::BeginPlay();
	

}


bool ACubeStorage::dropOffMaterial(Container* container)
{
	if (Super::dropOffMaterial(container)) {
		
		UStaticMeshComponent* compressedCube = NewObject<UStaticMeshComponent>(this);

		compressedCube->RegisterComponent();
		compressedCube->AttachToComponent(FirstSpawnSpot, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		AddOwnedComponent(compressedCube);
		compressedCube->SetStaticMesh(GarbageCubeMesh);
		compressedCube->SetMaterial(0, Materials[container->materialType - 4]);
		compressedCube->SetMaterial(1, secondMaterial);


		FVector newLocation = compressedCube->GetRelativeLocation();


		newLocation.Y += spawned > 2 ? -25 * (spawned - 3) : -25 * spawned;
		
		newLocation.Z = spawned > 2 ? 2250 : 400;

		spawned++;

		compressedCube->SetRelativeLocation(newLocation);

		FRotator newRotation = compressedCube->GetRelativeRotation();
		newRotation.Yaw += 90;

		compressedCube->SetRelativeRotation(newRotation);

		cubes.Add(compressedCube);

		return true;
	}
	return false;
}

void ACubeStorage::removeCubes()
{
	for (auto& cube : cubes) {
		cube->SetVisibility(false);
		cube->DestroyComponent();
	}
	spawned = 0;
	cubes.Empty();
}

void ACubeStorage::build()
{
	if (!inputBuilding) {
		spawnDropOff();
	}
}

void ACubeStorage::destroyBuilding()
{
	dropOffStation->Destroy();
	Super::destroyBuilding();
}


void ACubeStorage::spawnDropOff()
{
	dropOffStation = GetWorld()->SpawnActor<ADropOffStation>(dropOffClass);
	dropOffStation->owner = this;
	dropOffStation->isCompactCubes = true;

	dropOffStation->AttachToComponent(dropOffPoint, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

	FVector location = dropOffStation->GetActorLocation();
	location.Z = 0;
	dropOffStation->SetActorLocation(location);
}