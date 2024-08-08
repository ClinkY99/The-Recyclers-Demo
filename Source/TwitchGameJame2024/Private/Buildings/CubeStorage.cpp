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

	workStationPoint = CreateDefaultSubobject<USceneComponent>(FName("WorkStationLocation"));
	workStationPoint->SetupAttachment(mesh);
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

		SpawnTruck();

		return true;
	}
	return false;
}

void ACubeStorage::build()
{
	if (!inputBuilding) {
		spawnDropOff();
	}
	spawnWorkstation();
}

void ACubeStorage::SpawnTruck()
{
	int32 num = FMath::RandRange(0, 100);
	if (spawned == 4) {
		if (num < 10) {
			Cast<AMainCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))->SpawnCollectionTruck();
		}
	}
	if (spawned == 5) {
		if (num < 33) {
			Cast<AMainCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))->SpawnCollectionTruck();
		}
	} 
	if (spawned > 5) {
		Cast<AMainCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))->SpawnCollectionTruck();
	}
}


void ACubeStorage::spawnDropOff()
{
	ADropOffStation* dropOffStation = GetWorld()->SpawnActor<ADropOffStation>(dropOffClass);
	dropOffStation->owner = this;
	dropOffStation->isCompactCubes = true;

	dropOffStation->AttachToComponent(dropOffPoint, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

	FVector location = dropOffStation->GetActorLocation();
	location.Z = 0;
	dropOffStation->SetActorLocation(location);
}


void ACubeStorage::spawnWorkstation()
{
	AWorkstation* workstation = GetWorld()->SpawnActor<AWorkstation>(workstationClass);
	workstation->owner = this;

	workstation->AttachToComponent(workStationPoint, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

	FVector location = workstation->GetActorLocation();
	location.Z = 0;
	workstation->SetActorLocation(location);
}
